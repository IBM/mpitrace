/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mpi.h>

#define VMON_MAGIC   0x766d6f6e
#define VMON_VERSION 4

// Note: vprof expects binary data in big endian format
// byte-swapping is required for little endian systems
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
#define SWAP_BYTES 1
#endif

extern char _start;  // symbol for the start of program text
extern char _etext;  // symbol for the end of program text
extern char _fini;   // symbol for the end of program text

static int vprof_has_started = 0;

static unsigned short * buffer;
static long size;
static long offset;
static unsigned int outofrange = 0;
static unsigned int overflow = 0;
static char name[] = "PROF";

static int startup_failed = 0;

static int myrank = 0;

#pragma weak mpitrace_vprof_start_ = mpitrace_vprof_start
#pragma weak mpitrace_vprof_stop_  = mpitrace_vprof_stop 
#pragma weak mpitrace_vprof_write_ = mpitrace_vprof_write

// Note: vprof expects binary data in big endian format
// byte-swapping is required for little endian systems
#ifdef SWAP_BYTES
static void swap8_voff(long long *data)
{
  unsigned char tmp, *cdat = (unsigned char *) data;
  tmp = cdat[0]; cdat[0] = cdat[7], cdat[7] = tmp;
  tmp = cdat[1]; cdat[1] = cdat[6], cdat[6] = tmp;
  tmp = cdat[2]; cdat[2] = cdat[5], cdat[5] = tmp;
  tmp = cdat[3]; cdat[3] = cdat[4], cdat[4] = tmp;
}

static void swap4_uint(unsigned int *data)
{
  unsigned char tmp, *cdat = (unsigned char *) data;
  tmp = cdat[0]; cdat[0] = cdat[3], cdat[3] = tmp;
  tmp = cdat[1]; cdat[1] = cdat[2], cdat[2] = tmp;
}

static void swap2_ushort(unsigned short *data)
{
  unsigned char tmp, *cdat = (unsigned char *) data;
  tmp = cdat[0]; cdat[0] = cdat[1], cdat[1] = tmp;
}
#endif

static void bewrite_voff(FILE *fp, long long data)
{
#ifdef SWAP_BYTES
  swap8_voff(&data);
#endif
  fwrite(&data, 8, 1, fp);
}

static void bewrite_uint(FILE *fp, unsigned int data)
{
#ifdef SWAP_BYTES
  swap4_uint(&data);
#endif
  fwrite(&data, 4, 1, fp);
}

static void bewrite_uchar(FILE *fp, unsigned char data)
{
  fwrite(&data, 1, 1, fp);
}

static void bewrite_ushort(FILE *fp, unsigned short data)
{
#ifdef SWAP_BYTES
  swap2_ushort(&data);
#endif
  fwrite(&data, 2, 1, fp);
}


void mpitrace_vprof_start(void)
{
    int i;
    size_t nbytes;
    unsigned int scale = 0x10000;
    static int first_call = 1;

    if (first_call) {

       MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

       // YKT: on BGQ &_etext appears to work, but &_start does not, so use a fixed starting address
       // on Linux it looks like it is best to use &_fini instead of &_etext when using LD_PRELOAD
       size =  ((long) &_fini)- ((long) &_start);
       offset = (long) &_start;
       if (size < 0L) {
          startup_failed = 1;
          if (myrank == 0) printf("mpitrace_vprof_start : buffer size is negative ... returning\n");
          return;
       }

       nbytes = size*sizeof(unsigned short);
       buffer = (unsigned short *) malloc(nbytes);
       if (buffer == NULL) {
          if (myrank == 0) printf("mpitrace_vprof_start: malloc failed for the profile buffer ... returning\n");
          return;
       }
       else {
          for (i=0; i<size; i++) buffer[i] = (unsigned short) 0;
       }

       vprof_has_started = 1;
       first_call = 0;
    }

    profil(buffer, nbytes, offset, scale);

}


void mpitrace_vprof_stop(void)
{
   if (startup_failed || !vprof_has_started) return;
   profil(0,0,0,0);
}

void mpitrace_vprof_write(int jobid, int add_timestamp, timestamp)
{
   int i, nhit, count;
   char * trace_dir, filename[80];
   FILE * fp;

   if (startup_failed || !vprof_has_started) return;

   trace_dir = getenv("TRACE_DIR");

   count = 0;
   nhit  = 0;
   for (i=0; i<size; i++) {
      if (buffer[i]) {
          count++;
          nhit += buffer[i];
      }
   }

   if (myrank == 0) printf("vprof: collected %d hits at %d program-counter locations\n", nhit, count);

   // write data in a format that can be interpreted by cprof
   // "bewrite" is just fwrite in big endian format, forcing 8 bytes 
   // in the output file for the "offset" value
   if (add_timestamp) {
      if (trace_dir == NULL) sprintf(filename, "vmon.out.%d.%s.%d", jobid, timestamp, myrank);
      else                   sprintf(filename, "%s/vmon.out.%d.%s.%d", trace_dir, jobid, timestamp, myrank);
   }   
   else {
      if (trace_dir == NULL) sprintf(filename, "vmon.out.%d.%d", jobid, myrank);
      else                   sprintf(filename, "%s/vmon.out.%d.%d", trace_dir, jobid, myrank);
   }

   fp = fopen(filename, "wb");
   if (fp == NULL) {
      if (myrank == 0) printf("mpitrace_vprof_stop: failed to open %s ... returning\n", filename);
      return;
   }

   bewrite_uint(fp,  VMON_MAGIC);
   bewrite_uchar(fp, VMON_VERSION);
   bewrite_uint(fp, 1);

   for (i=0; name[i] != '\0'; i++) bewrite_uchar(fp, name[i]);
   bewrite_uchar(fp, '\0');

   bewrite_uint(fp, outofrange);
   bewrite_uint(fp, overflow);
   bewrite_uint(fp, count);
   bewrite_voff(fp, (long long) offset);       

   // each histogram bin takes 2 bytes of address space in program text
   for (i=0; i<size; i++) {
      if (buffer[i]) {
          bewrite_ushort(fp, buffer[i]);
          bewrite_uint(fp, i*2);
      }
   }

   fclose(fp);

}
