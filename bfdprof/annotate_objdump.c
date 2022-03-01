/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//=======================================================
// syntax : annotate_objdump executable vmon.out >asm.txt
//=======================================================

//=======================================================
// use predefined gcc macros to select architecture
//=======================================================

#if defined(__x86_64__)
 #define TARGET "x86_64-unknown-linux-gnu"
#elif defined(__PPC64__)
 #define TARGET "powerpc64-unknown-linux-gnu"
#elif defined(__aarch64__)
 #define TARGET "aarch64-unknown-linux-gnu"
#endif

#if  __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SWAP_BYTES 1   // for little endian systems
#endif

#define VMON_MAGIC   0x766d6f6e
#define VMON_VERSION 4

#define HISTO_MAGIC 0x13571357
#define HISTO_VERSION 1

#define SIZEPATH 1024  // size limit for sharedlib path
#define SIZEFILE 4096  // limit for file names (full path)
#define SIZELINE 4096  // limit for length of a line of objdump file
#define SIZEPREF 9     // size of the prefix region used for tics

#define EVENT_NAME_SIZE 128


struct Histo {
               unsigned long pc;
               int hits;
             };

struct Histo * histo;    // basic histogram data

static int get_vmon_histogram(char *);
static int get_histogram(char *);

#ifdef SWAP_BYTES
static void swap2(unsigned short, unsigned short *);
static void swap4(int, int *);
static void swap8(long long, long long *);
#endif

static void sortx(int * , int, int *, int);
static void annotate(char *, int);
static void annotate_vmon(char *, int);
static void print_help(void);

static int vmon_format = 0;


//================================================
// the main routine
//================================================
int main(int argc, char * argv[])
{
   unsigned long offset;
   char obj_file[SIZEFILE], exe_file[SIZEFILE], vmon_file[SIZEFILE];
   char command[320];
   const char * target = TARGET;
   int ch, i, j, k, magic, numpc, rc;
   struct stat statbuf;
   FILE * fp;

   if (argc == 1) {
      print_help();
      exit(0);
   }

   while ((ch = getopt (argc, argv, "?h")) != -1) {
      switch (ch) {
         case '?':
            print_help();
            exit(0);
            break;
         case 'h':
            print_help();
            exit(0);
            break;
         default:
            break;
      }
   }

   strcpy(exe_file,  argv[optind]);
   strcpy(vmon_file, argv[optind+1]);

   printf("Using executable file : %s, histogram file : %s.\n", exe_file, vmon_file);

   sprintf(command, "objdump --prefix-addresses -j .text -d %s > %s.dump", exe_file, exe_file);

   system(command);

   sprintf(obj_file, "%s.dump", exe_file);

   // check histogram-data file format
   fp = fopen(vmon_file, "r");
   if (fp == NULL) {
      printf(" failed to open histogram data file %s ... exiting\n", vmon_file);
      exit(-1);
   }

   // read the histogram
   rc = fread(&magic, sizeof(int), 1, fp);
   if (magic == HISTO_MAGIC) numpc = get_histogram(vmon_file);
   else                      numpc = get_vmon_histogram(vmon_file);
   fclose(fp);

   //==================================================
   // annotate the objdump file
   //==================================================
   rc = stat(obj_file, &statbuf);
   if (rc == 0) {
      if (vmon_format) annotate_vmon(obj_file, numpc);
      else             annotate(obj_file, numpc);
   }

   return 0;
}


//================================================
// routine to print a help message
//================================================
static void print_help(void)
{
   printf("syntax : annotate_objdump [-h] executable vmon.out.N\n");
   printf("options : -h   => help\n");
}

//================================================
// routine to annotate source files with hits
//================================================
static void annotate(char * obj_file, int numpc)
{
   int fd;
   struct stat statbuf;
   size_t filesize, nbytes;
   char prefmt[8], fmt[8], prefix[SIZEPREF], hlabel[SIZEPREF], line[SIZELINE];
   char hfmt[16], heading[32];
   char address[40], faddr[40];
   int i, j, rc, jstart, numlines, indx, beg;
   char * filebuf, * hitstr;
   int * linepos, * linelen;  
   long hbin, fbin;
   char fstring[32];

   char objdump_address[16], adjusted_objdump_address[16];
   int adjusted_integer_objdump_address;

   fd = open(obj_file, O_RDONLY);
   if (fd < 0) {
     printf("open failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   rc = fstat(fd, &statbuf);
   if (rc !=  0) {
     printf("fstat failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   filesize = statbuf.st_size;

   filebuf = (char *) malloc((filesize+1)*sizeof(char));

   nbytes = read(fd, filebuf, filesize);
   if (nbytes != filesize) {
     printf("read failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   filebuf[filesize] = '\0';

   rc = close(fd);

   numlines = 0;
   for (i=0; i<filesize; i++) {
      if (filebuf[i] == '\n') numlines++;
   }

   linepos = (int *) malloc((numlines+1)*sizeof(int));
   linelen = (int *) malloc(numlines*sizeof(int));
   hitstr = (char *) malloc(numlines*SIZEPREF*sizeof(char));

   memset(hitstr, '\0', numlines*SIZEPREF);

   sprintf(prefmt, "%%%ds",  SIZEPREF-1);
   sprintf(prefix, prefmt, "|\0");
   
   for (i=0; i<numlines; i++) {
      strcpy(&hitstr[i*SIZEPREF], prefix);
   }

   sprintf(fmt, "%c%d%c%c", '%', SIZEPREF-2, 'd', '|');

   linepos[0] = 0;
   numlines = 0;
   beg =0;
   for (i=0; i<filesize; i++) {
      if (filebuf[i] == '\n') {
         linelen[numlines] = i - beg + 1;
         numlines++;
         linepos[numlines] =  i + 1;
         beg = i + 1;
      }
   }
   linelen[numlines-1] = filesize - linepos[numlines-1] + 1;

   jstart = 0;
   for (i=0; i<numpc; i++) {
      sprintf(address, "%016lx", histo[i].pc);
      // now find the line in the objdump that matches the address
      for (j=jstart; j<numlines; j++) {
        if (strncmp(filebuf + linepos[j], address, 16) == 0) {
           sprintf(hlabel, fmt, histo[i].hits);
           strncpy(&hitstr[j*SIZEPREF], hlabel, sizeof(hlabel));
           jstart = j;
           break;
        }
      }
   }

   sprintf(hfmt, "%%%ds | %%s", SIZEPREF-3);
   sprintf(heading, hfmt, "tics", "source \n");

   printf("\n");
   printf("##########################\n");
   printf("Annotated objdump file: %s\n", obj_file);
   printf("##########################\n");
   printf("%s", heading);
   for (i=0; i<numlines; i++) {
      memset(line, '\0', SIZELINE);
      indx = linepos[i];
      strncpy(line, &hitstr[i*SIZEPREF], SIZEPREF);
      strncpy(&line[SIZEPREF-1], &filebuf[indx], linelen[i]);
      printf("%s", line);
   }
   printf("\n");

   free(filebuf);
   free(linepos);
   free(linelen);
   free(hitstr);
   
   return;
}

//===================================================
// routine to annotate source files with hits : vprof
//===================================================
static void annotate_vmon(char * obj_file, int numpc)
{
   int fd;
   struct stat statbuf;
   size_t filesize, nbytes;
   char prefmt[8], fmt[8], prefix[SIZEPREF], hlabel[SIZEPREF], line[SIZELINE];
   char hfmt[16], heading[32];
   char address[40], faddr[40];
   int i, j, rc, jstart, numlines, indx, beg;
   char * filebuf, * hitstr;
   int * linepos, * linelen;  
   long hbin, fbin;
   char fstring[32];

   long target_pc;

   fd = open(obj_file, O_RDONLY);
   if (fd < 0) {
     printf("open failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   rc = fstat(fd, &statbuf);
   if (rc !=  0) {
     printf("fstat failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   filesize = statbuf.st_size;

   filebuf = (char *) malloc((filesize+1)*sizeof(char));

   nbytes = read(fd, filebuf, filesize);
   if (nbytes != filesize) {
     printf("read failed for %s ... exiting\n", obj_file);
     exit(0);
   }

   filebuf[filesize] = '\0';

   rc = close(fd);

   numlines = 0;
   for (i=0; i<filesize; i++) {
      if (filebuf[i] == '\n') numlines++;
   }

   linepos = (int *) malloc((numlines+1)*sizeof(int));
   linelen = (int *) malloc(numlines*sizeof(int));
   hitstr = (char *) malloc(numlines*SIZEPREF*sizeof(char));

   memset(hitstr, '\0', numlines*SIZEPREF);

   sprintf(prefmt, "%%%ds",  SIZEPREF-1);
   sprintf(prefix, prefmt, "|\0");
   
   for (i=0; i<numlines; i++) {
      strcpy(&hitstr[i*SIZEPREF], prefix);
   }

   sprintf(fmt, "%c%d%c%c", '%', SIZEPREF-2, 'd', '|');

   linepos[0] = 0;
   numlines = 0;
   beg =0;
   for (i=0; i<filesize; i++) {
      if (filebuf[i] == '\n') {
         linelen[numlines] = i - beg + 1;
         numlines++;
         linepos[numlines] =  i + 1;
         beg = i + 1;
      }
   }
   linelen[numlines-1] = filesize - linepos[numlines-1] + 1;

   jstart = 0;
   for (i=0; i<numpc; i++) {
#if defined(__x86_64__) || defined(__aarch64__)
      if (0x00000000F0000000 | histo[i].pc == 0L) sprintf(address, "0000000000%lx", histo[i].pc);
      else                                        sprintf(address, "000000000%lx", histo[i].pc);
#else
      sprintf(address, "00000000%lx", histo[i].pc);
#endif
      // now find the line in the objdump that matches the address
      // for vmon format check if the address is within a histogram bin = +/- 1
      for (j=jstart; j<numlines; j++) {
        target_pc = strtol(filebuf + linepos[j], NULL, 16);
        if (abs(target_pc - histo[i].pc) < 2) {
           sprintf(hlabel, fmt, histo[i].hits);
           strncpy(&hitstr[j*SIZEPREF], hlabel, sizeof(hlabel));
           jstart = j;
           break;
        }
      }
   }

   sprintf(hfmt, "%%%ds | %%s", SIZEPREF-3);
   sprintf(heading, hfmt, "tics", "source \n");

   printf("\n");
   printf("##########################\n");
   printf("Annotated objdump file: %s\n", obj_file);
   printf("##########################\n");
   printf("%s", heading);
   for (i=0; i<numlines; i++) {
      memset(line, '\0', SIZELINE);
      indx = linepos[i];
      strncpy(line, &hitstr[i*SIZEPREF], SIZEPREF);
      strncpy(&line[SIZEPREF-1], &filebuf[indx], linelen[i]);
      printf("%s", line);
   }
   printf("\n");

   free(filebuf);
   free(linepos);
   free(linelen);
   free(hitstr);
   
   return;
}


//======================================================
// routine to get the histogram data in histo.dat format
//======================================================
static int get_histogram(char * vmonfile)
{
   int i, k, bin, hits, magic, version, numaddr, histo_size, tothits;
   int rc, event, threshold, unknown, len;
   unsigned long offset;
   char * sharedlib_name;
   int * sharedlib_hits;
   int * sharedlib_indx;
   int nsharedlibs = 0;
   char eventName[EVENT_NAME_SIZE];
   FILE * fp;

   fp = fopen(vmonfile, "r");
   if (fp == NULL) {
      printf("failed to open %s ... exiting\n", vmonfile);
      exit(0);
   }

   rc = fread(&magic,      sizeof(int),       1, fp);
   if (magic != HISTO_MAGIC) {
      printf("%s file format not recognized ... exiting\n", vmonfile);
      exit(-1);
   }
   rc = fread(&version,    sizeof(int),       1, fp);
   rc = fread(&eventName,  sizeof(eventName), 1, fp);
   rc = fread(&threshold,  sizeof(int),       1, fp);
   rc = fread(&numaddr,    sizeof(int),       1, fp);
   rc = fread(&histo_size, sizeof(int),       1, fp);
   rc = fread(&offset,     sizeof(long),      1, fp);

   histo = (struct Histo *) malloc(numaddr*sizeof(struct Histo));

   // each instruction takes four bytes, so ...
   // the address increments by four for every histogram bin
   tothits = 0;
   for (i=0; i<numaddr; i++) {
      rc = fread(&bin,  sizeof(int), 1, fp);
      rc = fread(&hits, sizeof(int), 1, fp);
      histo[i].hits = hits;
#ifdef __x86_64__ 
      histo[i].pc = offset + bin;
#else
      histo[i].pc = offset + (bin << 2);
#endif
      tothits += hits;
   }

   // read data for shared libs
   rc = fread(&unknown, sizeof(int), 1, fp);
   rc = fread(&nsharedlibs, sizeof(int), 1, fp);
   if (nsharedlibs > 0) {
      sharedlib_hits = (int *) malloc(nsharedlibs*sizeof(int));
      sharedlib_indx = (int *) malloc(nsharedlibs*sizeof(int));
      sharedlib_name = (char *) malloc(nsharedlibs*SIZEPATH);
      memset(sharedlib_name, '\0', nsharedlibs*SIZEPATH);
      for (i=0; i<nsharedlibs; i++) {
         rc = fread(&sharedlib_hits[i], sizeof(int),  1, fp);
         rc = fread(&len, sizeof(int), 1, fp);
         rc = fread(&sharedlib_name[i*SIZEPATH], len, 1, fp);
      }
   }
   fclose(fp);

   printf("%10d hits at %d program-text locations,\n", tothits, numaddr);
   printf("%10d hits outside the program-text section,\n", unknown);
   printf("%10d hits total.\n", unknown + tothits);
   printf("HPM sampling using event = %s, threshold = %d.\n", eventName, threshold);

   // sort sharedlibs by hits in decreasing order
   if (nsharedlibs > 0) {
      sortx(sharedlib_hits, nsharedlibs, sharedlib_indx, -1);
      printf("\n");
      printf("##########################\n");
      printf("Shared library profile:\n");
      printf("##########################\n");
      printf("      tics   sharedlib\n");
      printf("--------------------------\n");
      for (i=0; i<nsharedlibs; i++) {
         k = sharedlib_indx[i];
         printf("%10d   %s\n", sharedlib_hits[i], &sharedlib_name[k*SIZEPATH]);
      }
      printf("\n");
   }

   return numaddr;
}

//======================================================
// routine to get the histogram data in vmon.out format
//======================================================
static int get_vmon_histogram(char * vmonfile)
{
   FILE * fp;
   size_t rc;
   char name[4];
   int one, outofrange, overflow;
   unsigned short hitsbe, hits;
   int magicbe, magic;
   int numpcbe, numpc;
   int relpcbe, relpc;
   int i, tothits;
   long long offsetbe, offset;
   char version, chzero;

   fp = fopen(vmonfile, "r");
   if (fp == NULL) {
      printf("failed to open %s ... exiting\n", vmonfile);
      exit(0);
   }

#ifdef SWAP_BYTES
   rc = fread(&magicbe,    1,            sizeof(int),  fp);
   swap4(magicbe, &magic);
#else
   rc = fread(&magic,      1,            sizeof(int),  fp);
#endif
   if (magic != VMON_MAGIC) {
      printf("vmon.out file format is not recognized ... exiting\n");
      exit(0);
   } 
   vmon_format = 1;
   rc = fread(&version,    1,            sizeof(char), fp);
   if (version != VMON_VERSION) {
      printf("vmon.out file format is not recognized ... exiting\n");
      exit(0);
   } 
   rc = fread(&one,        1,            sizeof(int),  fp);
   rc = fread(name,        sizeof(name), sizeof(char), fp);
   rc = fread(&chzero,     1,            sizeof(char), fp);
   rc = fread(&outofrange, 1,            sizeof(int),  fp);
   rc = fread(&overflow,   1,            sizeof(int),  fp);
#ifdef SWAP_BYTES
   rc = fread(&numpcbe,    1,            sizeof(int),  fp);
   swap4(numpcbe, &numpc);
   rc = fread(&offsetbe,   1,            sizeof(long long), fp);
   swap8(offsetbe, &offset);
#else
   rc = fread(&numpc,      1,            sizeof(int),  fp);
   rc = fread(&offset,     1,            sizeof(long long), fp);
#endif
// printf("got numpc = %d, offset = %ld\n", numpc, offset);

   histo = (struct Histo *) malloc(numpc*sizeof(struct Histo));

   tothits = 0;
   for (i=0; i<numpc; i++) {
#ifdef SWAP_BYTES
      rc = fread(&hitsbe,  1, sizeof(unsigned short), fp);
      swap2(hitsbe, &hits);
      rc = fread(&relpcbe, 1, sizeof(int),   fp);
      swap4(relpcbe, &relpc);
#else
      rc = fread(&hits,    1, sizeof(unsigned short), fp);
      rc = fread(&relpc,   1, sizeof(int),   fp);
#endif
      histo[i].pc   = offset + relpc;
      histo[i].hits = (int) hits;
      tothits += hits;
   }
   printf("Got a total of %d hits at %d program-counter locations,\n", tothits, numpc);
   printf("corresponding to a cpu time of about %.2lf seconds.\n", 1.0e-2*((double) tothits));

   fclose(fp);

   return numpc;
}


//================================================
// routine to sort and return the index array
//================================================
static void sortx(int * arr , int n, int * ind, int flag)
{
   int h, i, j, k, inc[20];
   int numinc, pwr2, pwr4;
   int val;

   if (n <= 1) {
      ind[0] = 0;
      return;
   }

   pwr2 = 1;
   pwr4 = 4;

   numinc = 0;
   h = 1;
   inc[numinc] = h;
   while (numinc < 20) {
      h = 1 + 3*pwr2 + pwr4;
      if (h > n) break;
      numinc++;
      inc[numinc] = h;
      pwr2 *= 2;
      pwr4 *= 4;
   }

   for (i=0; i<n; i++) ind[i] = i;

   if (flag > 0) { // sort in increasing order 
      for (; numinc >= 0; numinc--) {
         h = inc[numinc];
         for (i = h; i < n; i++) {
            val = arr[i];
            k   = ind[i];

            j = i;
   
            while ( (j >= h) && (arr[j-h] > val) ) {
               arr[j] = arr[j-h];
               ind[j] = ind[j-h];
               j = j - h;
            }

            arr[j] = val;
            ind[j] = k;
         }
      }
   }
   else { // sort in decreasing order 
      for (; numinc >= 0; numinc--) {
         h = inc[numinc];
         for (i = h; i < n; i++) {
            val = arr[i];
            k   = ind[i];

            j = i;
   
            while ( (j >= h) && (arr[j-h] < val) ) {
               arr[j] = arr[j-h];
               ind[j] = ind[j-h];
               j = j - h;
            }

            arr[j] = val;
            ind[j] = k;
         }
      }
   }
}

#ifdef SWAP_BYTES
//================================================
// routine to byte-swap a unsigned short int
//================================================
static void swap2(unsigned short be, unsigned short * le)
{
   char * in, * out;
   in  = (char *) &be;
   out = (char *) le;
   out[1] = in[0];
   out[0] = in[1];
}


//================================================
// routine to byte-swap an int
//================================================
static void swap4(int be, int * le)
{
   char * in, * out;
   in  = (char *) &be;
   out = (char *) le;
   out[3] = in[0];
   out[2] = in[1];
   out[1] = in[2];
   out[0] = in[3];
}


//================================================
// routine to byte-swap a long
//================================================
static void swap8(long long be, long long * le)
{
   char * in, * out;
   in  = (char *) &be;
   out = (char *) le;
   out[7] = in[0];
   out[6] = in[1];
   out[5] = in[2];
   out[4] = in[3];
   out[3] = in[4];
   out[2] = in[5];
   out[1] = in[6];
   out[0] = in[7];
}
#endif
