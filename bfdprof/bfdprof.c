/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <config.h>   // for binutils 2.23.1 and later
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <bfd.h>
#include <demangle.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//===================================================
// syntax : bfdprof executable vmon.out > profile.txt
//===================================================

//======================================================
// use predefined gcc macros to select architecture
//======================================================
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
#define SIZELINE 4096  // limit for length of a line of source code
#define SIZEFILE 4096  // limit for file names (full path)
#define SIZEFUNC 16384 // limit for function names (mangled)
#define SIZEPRNT 128   // print no more than 128 characters of the function name
#define SIZEPREF 9     // size of the prefix region used for tics

#define EVENT_NAME_SIZE 128

static asymbol **symtab; // symbol table

struct Histo {
               bfd_vma pc;
               int hits;
             };

struct Histo * histo;    // basic histogram data

struct Prof {
               char file[SIZEFILE];
               char func[SIZEFUNC];
               bfd_vma pc;
               int hits;
               int line;
            };

struct Prof * prof;     // profile data

struct LineData {
                   int line;
                   int hits;
                };

static void get_symtab(bfd *);
static int  get_vmon_histogram(char *);
static int  get_histogram(char *);
#ifdef SWAP_BYTES
static void swap2(unsigned short, unsigned short *);
static void swap4(int, int *);
static void swap8(long long, long long *);
#endif
static void sortx(int * , int, int *, int);
static void annotate(char *, struct LineData *, int);
static void print_help(void);


//================================================
// the main routine
//================================================
int main(int argc, char * argv[])
{
   struct LineData * line_data;
   bfd_vma offset;
   bfd * bfdfile = NULL;
   sec_ptr section;
   char exe_file[SIZEFILE], vmon_file[SIZEFILE];
   char lfmt[16], hfmt[16], heading[32];
   const char * target = TARGET;
   int ch, i, j, k, magic, numpc, maxfuncs, istart, maxlines, match, ppc64, rc;
   int * function_index, * file_index;
   int numroutines, hitlines, numfiles, ifile, myfile;
   int * function_tics, * file_tics, * file_start;
   char * function_name, * file_name;
   int verbose_flag = 0;
   int routine_flag = 0;
   int demangle_flag = 1;
   int demangle_opt = 0;
   int inline_opt = -1;
   int annotate_cutoff = 3;
   size_t bufsize;
   struct stat statbuf;
   FILE * fp;

   if (argc == 1) {
      print_help();
      exit(0);
   }

   while ((ch = getopt (argc, argv, "?hi:vrc:d:")) != -1) {
      switch (ch) {
         case '?':
            print_help();
            exit(0);
            break;
         case 'h':
            print_help();
            exit(0);
            break;
         case 'i':
            inline_opt = atoi(optarg);
            break;
         case 'v':
            verbose_flag = 1;
            break;
         case 'r':
            routine_flag = 1;
            break;
         case 'c':
            annotate_cutoff = atoi(optarg);
            break;
         case 'd':
            demangle_opt = atoi(optarg);
            if (demangle_opt == -1) demangle_flag = 0;
            else                    demangle_flag = 1;
            break;
         default:
            break;
      }
   }

   strcpy(exe_file,  argv[optind]);
   strcpy(vmon_file, argv[optind+1]);

   printf("Using executable file : %s, histogram file : %s.\n", exe_file, vmon_file);

   bfd_init ();
   if (!bfd_set_default_target(target)) {
      printf("bfd_set_default_target failed ... exiting\n");
      exit(0);
   }

   bfdfile = bfd_openr(exe_file, target);
   if (bfdfile == NULL) {
      printf("bfd_openr failed for %s ... exiting\n", exe_file);
      exit(0);
   }

   if (!bfd_check_format(bfdfile, bfd_object)) {
      printf("problem with executable format ... exiting\n");
   }

   section = bfd_get_section_by_name(bfdfile, ".text");
   if (section == NULL) {
      printf("bfd_get_section_by_name failed ... exiting\n");
      exit(0);
   }

   offset = section->vma;

   get_symtab(bfdfile);

   // check histogram-data file format
   fp = fopen(vmon_file, "r");
   if (fp == NULL) {
      printf(" failed to open histogram data file %s ... exiting\n", vmon_file);
      exit(-1);
   }
   rc = fread(&magic, sizeof(int), 1, fp);
   if (magic == HISTO_MAGIC) numpc = get_histogram(vmon_file);
   else                      numpc = get_vmon_histogram(vmon_file);
   fclose(fp);

   prof = (struct Prof *) malloc(numpc*sizeof(struct Prof));

   for (i=0; i<numpc; i++) {
      memset(prof[i].file, '\0', SIZEFILE);
      memset(prof[i].func, '\0', SIZEFUNC);
   }

   if (strcmp(TARGET, "powerpc64-unknown-linux-gnu") == 0) ppc64 = 1;
   else                                                    ppc64 = 0;

   //====================================================
   // loop over program counters translating each address
   //====================================================
   for (i=0; i<numpc; i++) {
      const char *filename;
      const char *functionname;
      unsigned int line;
      int unwind_count;
      bfd_boolean found;
      bfd_vma pc;

      pc = (bfd_vma) histo[i].pc;

      line = 0;
      filename = NULL;
      functionname = NULL;

      found = bfd_find_nearest_line(bfdfile, section, symtab, pc - offset,
                                    &filename, &functionname, &line);

      // unwind inlined functions by default ... but offer some control via the -i option
      if (inline_opt == -1) {
         do { found = bfd_find_inliner_info(bfdfile, &filename, &functionname, &line);
         } while(found);
      }   
      else if (inline_opt > 0) {
         unwind_count = 0;
         do { found = bfd_find_inliner_info(bfdfile, &filename, &functionname, &line);
              unwind_count++;
         } while(found && (unwind_count < inline_opt));
      }

      if (verbose_flag) printf("pc = %#10.8lx, hits = %d, func = %s, location = %s:%d\n", 
                         (unsigned long) pc, histo[i].hits, functionname, filename, line);

      prof[i].pc   = histo[i].pc;
      prof[i].hits = histo[i].hits;
      prof[i].line = line;
      if (NULL == filename) {
         strcpy(prof[i].file, "unknown");
      }
      else {
         // hack for failure to find files for powerpc64 architecture
         if (ppc64 && (filename[0] != '/')) strcpy(prof[i].file, "unknown"); 
         else                               strcpy(prof[i].file, filename);
      }
      if (NULL == functionname) strcpy(prof[i].func, "unknown");
      else                      strcpy(prof[i].func, functionname);

   }

   //====================================================
   // reduce data to obtain the function-level profile
   // limit the function-name buffer to a max of 512 MB
   // this corresponds to 32K routines with 16KB SIZEFUNC
   //====================================================
   function_tics = (int *) malloc(numpc*sizeof(int));
   maxfuncs = 512 * (1024 * 1024 / SIZEFUNC);
   if (numpc < maxfuncs) maxfuncs = numpc;
   bufsize = ((size_t) maxfuncs) *  ((size_t) SIZEFUNC);
   function_name = (char *) malloc(bufsize);
   memset(function_name, '\0', bufsize);

   for (i=0; i<numpc; i++) function_tics[i] = 0;
   
   numroutines = 0;
   for (i=0; i<numpc; i++) {
      match = 0;
      for (k=numroutines-1; k>=0; k--) {
         if (strcmp(prof[i].func, &function_name[k*SIZEFUNC]) == 0) {
            function_tics[k] += prof[i].hits;
            match = 1;
            break;
         }
      }
      if (match == 0) {
         strcpy(&function_name[numroutines*SIZEFUNC], prof[i].func);
         function_tics[numroutines] += prof[i].hits;
         numroutines++;
         if (numroutines > maxfuncs) {
            printf("bfdprof : function-name buffer overflow ... exiting.\n");
            exit(0);
         }
      }
   }

   //==================================================
   // sort the function-level profile (decreasing order)
   //==================================================
   function_index = (int *) malloc(numroutines*sizeof(int));

   sortx(function_tics, numroutines, function_index, -1); 

   // format strings for output
   sprintf(lfmt, "%%%dd  %%s \n", SIZEPREF-1);
   sprintf(hfmt, "%%%ds   %%s", SIZEPREF-1);
   sprintf(heading, hfmt, "tics", "function-name \n");

   printf("\n");
   printf("########################################\n");
   printf("Function-level profile (exclusive data):\n");
   printf("########################################\n");
   printf("%s", heading);
   printf("--------------------------\n");
   for (i=0; i<numroutines; i++) {
      char * xlomp, * ptr, * fptr, * demangled = NULL;
      int len;
      k = function_index[i];
// allow demangle options disabled=-1, DMGL_NO_OPTS=0, DMGL_PARAMS=1
// DMGL_PARAMS shows argument lists making the demangled name very long   
// DMGL_NO_OPTS (default) does not show args and is thus easier to read
      if (!demangle_flag) fptr = &function_name[k*SIZEFUNC];
      else {
         demangled = bfd_demangle(bfdfile, &function_name[k*SIZEFUNC], demangle_opt);
         if (demangled == NULL) fptr = &function_name[k*SIZEFUNC];
         else {
            fptr = demangled;
            // IBM XL OpenMP name-mangling adds $$OL$$... to the end of the function name
            // if demangling has removed the IBM XL OpenMP decoration, add it back on
            xlomp = strstr(&function_name[k*SIZEFUNC], "$$OL$$");
            if (xlomp) {
               ptr = strstr(fptr, "$$OL$$");
               if (ptr == NULL) {
                  len = strlen(fptr);
                  strcat(&fptr[len-1], xlomp);
               }
            }
         }
      }

      printf(lfmt, function_tics[i], fptr);
   }
   printf("\n");

   free(function_index);
   free(function_name);
   free(function_tics);

   //==================================================
   // reduce data to obtain the file-level profile
   //==================================================
   file_tics = (int *) malloc(numpc*sizeof(int));
   file_name = (char *) malloc(numpc*SIZEFILE);
   memset(file_name, '\0', numpc*SIZEFILE);

   for (i=0; i<numpc; i++) file_tics[i] = 0;
   
   numfiles = 0;
   for (i=0; i<numpc; i++) {
      match = 0;
      for (k=numfiles-1; k>=0; k--) {
         if (strcmp(prof[i].file, &file_name[k*SIZEFILE]) == 0) {
            file_tics[k] += prof[i].hits;
            match = 1;
            break;
         }
      }
      if (match == 0) {
         strcpy(&file_name[numfiles*SIZEFILE], prof[i].file);
         file_tics[numfiles] += prof[i].hits;
         numfiles ++;
      }
   }

   //==================================================
   // sort the file-level profile (decreasing order)
   //==================================================
   file_index = (int *) malloc(numfiles*sizeof(int));

   sortx(file_tics, numfiles, file_index, -1); 

   sprintf(hfmt, "%%%ds   %%s", SIZEPREF-1);
   sprintf(heading, hfmt, "tics", "source-file \n");

   printf("\n");
   printf("##########################\n");
   printf("Source-file profile:\n");
   printf("##########################\n");
   printf("%s", heading);
   printf("--------------------------\n");
   for (i=0; i<numfiles; i++) {
      k = file_index[i];
      printf(lfmt, file_tics[i], &file_name[k*SIZEFILE]);
   }
   printf("\n");

   if (routine_flag) goto finish;

   //==================================================
   // annotate source files using the histogram data
   //==================================================
   line_data = (struct LineData *) malloc(numpc*sizeof(struct LineData));

   for (ifile=0; ifile<numfiles; ifile++) {
      char * source_file;
      j = file_index[ifile];
      source_file = &file_name[j*SIZEFILE];
      // annotate files with more than some number of hits
      if (file_tics[ifile] > annotate_cutoff) {
         hitlines = 0;
         for (i=0; i<numpc; i++) {
            if (strcmp(source_file, prof[i].file) == 0) {
               match = 0;
               for (k=0; k<hitlines; k++) {
                  if (prof[i].line == line_data[k].line) {
                     line_data[k].hits += prof[i].hits;
                     match = 1;
                     break;
                  }
               }
               if (match == 0) {
                  line_data[hitlines].line = prof[i].line;
                  line_data[hitlines].hits = prof[i].hits;
                  hitlines ++;
               }
            }
         }
         rc = stat(source_file, &statbuf);
         if (rc == 0) annotate(source_file, line_data, hitlines);
      }
   }

   free(line_data);
finish:
   free(file_name);
   free(file_tics);
   free(file_index);
   free(prof);
   free(histo);

   return 0;
}


//================================================
// routine to print a help message
//================================================
static void print_help(void)
{
   printf("syntax : bfdprof [-h] [-i n] [-v] [-r] [-c n] [-d n] executable vmon.out.N > profile.txt\n");
   printf("options : -h   => help\n");
   printf("          -i n => attempt to unwind inline functions to depth n ... n = 0 means no unwinding\n");
   printf("          -v   => verbose, lists all histogram addresses with hits\n");
   printf("          -r   => routines only ... skip source annotation\n");
   printf("          -c n => annotate source files with >n hits\n");
   printf("          -d n => demangle options n=-1,0,1; -1=>disabled, 0=>no-args, 1=>args\n");
}

//================================================
// routine to annotate source files with hits
//================================================
static void annotate(char * source_file, struct LineData * line_data, int hitlines)
{
   int fd;
   struct stat statbuf;
   size_t filesize, nbytes;
   char prefmt[8], fmt[8], prefix[SIZEPREF], hlabel[SIZEPREF], line[SIZELINE];
   char hfmt[16], heading[32];
   int i, rc, numlines, indx, beg;
#ifdef DYNAMIC_ARRAYS 
   char * filebuf, * hitstr;
   int * linepos, * linelen;  
#else
   static char filebuf[8000000], hitstr[8000000];
   static int linepos[1000000], linelen[1000000];
#endif

   fd = open(source_file, O_RDONLY);
   if (fd < 0) return;

   rc = fstat(fd, &statbuf);
   if (rc !=  0) return;

   filesize = statbuf.st_size;

#ifdef DYNAMIC_ARRAYS 
   filebuf = (char *) malloc((filesize+1)*sizeof(char));
#endif

   nbytes = read(fd, filebuf, filesize);
   if (nbytes != filesize) return;

   filebuf[filesize] = '\0';

   rc = close(fd);

   numlines = 0;
   for (i=0; i<filesize; i++) {
      if (filebuf[i] == '\n') {
         numlines++;
      }
   }

#ifdef DYNAMIC_ARRAYS
   linepos = (int *) malloc((numlines+1)*sizeof(int));
   linelen = (int *) malloc(numlines*sizeof(int));
   hitstr = (char *) malloc(numlines*SIZEPREF*sizeof(char));
#endif

   memset(hitstr, '\0', numlines*SIZEPREF);

   sprintf(prefmt, "%%%ds",  SIZEPREF-1);
   sprintf(prefix, prefmt, "|\0");
   
   for (i=0; i<numlines; i++) {
      strcpy(&hitstr[i*SIZEPREF], prefix);
   }

   sprintf(fmt, "%c%d%c%c", '%', SIZEPREF-2, 'd', '|');

   for (i=0; i<hitlines; i++) {
     indx = line_data[i].line - 1;
     sprintf(hlabel, fmt, line_data[i].hits);  
     strncpy(&hitstr[indx*SIZEPREF], hlabel, sizeof(hlabel));
   }

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

   sprintf(hfmt, "%%%ds | %%s", SIZEPREF-3);
   sprintf(heading, hfmt, "tics", "source \n");

   printf("\n");
   printf("##########################\n");
   printf("Annotated source for file: %s\n", source_file);
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

#ifdef DYNAMIC_ARRAYS
   free(filebuf);
   free(linepos);
   free(linelen);
   free(hitstr);
#endif
   
   return;
}


//================================================
// routine to get the symbol table
//================================================
static void get_symtab(bfd *abfd)
{
   long storage;
   long symcount;
   bfd_boolean dynamic = FALSE;

   if ((bfd_get_file_flags(abfd) & HAS_SYMS) == 0) {
      printf("problems with the symbol table ... exiting\n");
      exit(0);
   }

   storage = bfd_get_symtab_upper_bound(abfd);
   if (storage == 0) {
      storage = bfd_get_dynamic_symtab_upper_bound(abfd);
      dynamic = TRUE;
   }

   if (storage < 0) {
      printf("problems with the symbol table ... exiting\n");
      exit(0);
   }

   symtab = (asymbol **) malloc (storage);
 
   if (dynamic) symcount = bfd_canonicalize_dynamic_symtab(abfd, symtab);
   else         symcount = bfd_canonicalize_symtab(abfd, symtab);

   if (symcount < 0) {
      printf("problems with the symbol table ... exiting\n");
      exit(0);
   }

}


//======================================================
// routine to get the histogram data in histo.dat format
//======================================================
static int get_histogram(char * vmonfile)
{
   int i, k, bin, hits, magic, version, numaddr, histo_size, tothits;
   int rc, event, threshold, unknown, len;
   long offset;
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
      histo[i].pc = (bfd_vma) offset + (bfd_vma) bin;
#else
      histo[i].pc = (bfd_vma) offset + (bfd_vma) (bin << 2);
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
      histo[i].pc   = (bfd_vma) offset + (bfd_vma) relpc;
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
