/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#define _GNU_SOURCE  // required for dladdr()
#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <bfd.h>
#include <string.h>
#include <dlfcn.h>
#include <papi.h>
#ifndef SERIAL_VERSION
#include <mpi.h>
#endif
#include <sys/ucontext.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>

#if defined(__x86_64__)
 #define TARGET "x86_64-unknown-linux-gnu"
// #define DEFAULT_EVENT "perf::ref-cycles"  
 #define DEFAULT_EVENT "perf::cycles"
#elif defined(__PPC64__)
 #define TARGET "powerpc64-unknown-linux-gnu"
 #define DEFAULT_EVENT "perf::cycles"
#elif defined(__aarch64__)
 #define TARGET "aarch64-unknown-linux-gnu"
 #define DEFAULT_EVENT "CPU_CYCLES"
#endif

// -------------------------------------------------
// link with libhpmprof.so and libpapi.a etc.
// -------------------------------------------------
// C or  C++ routines to control the sampled region
// -------------------------------------------------
// HPM_Prof_init();       for the serial version
// HPM_Prof_start();
// do_work();
// HPM_Prof_stop();
// HPM_Prof_write();      for the serial version
// -------------------------------------------------
// Fortran interface to control the sampled region
// -------------------------------------------------
// call hpm_prof_init()   for the serial version
// call hpm_prof_start()
// call do_work()
// call hpm_prof_stop()
// call hpm_prof_write()  for the serial version
// -------------------------------------------------

#pragma weak hpm_prof_start_=hpm_prof_start
#pragma weak hpm_prof_stop_=hpm_prof_stop

#define HISTO_MAGIC 0x13571357
#define HISTO_VERSION 1
#define EVENT_NAME_SIZE 128

static int * histogram;           // holds hits for each instruction address

extern char _start;               // symbol for the start of program text
extern char _etext;               // symbol for the end of program text
extern char _fini;                // symbol for the end of program text
static long histo_size;           // number of histogram bins
static long offset;           

static int initialized = 0;
static int autostart = 0;
static int eventHandle = PAPI_NULL;

static int eventCode;
static char eventName[EVENT_NAME_SIZE];       // "perf::cycles"
#if defined(__x86_64__)
static int threshold = 27000000;              // adjust for ~100 samples/sec 
#elif defined(__aarch64__)
static int threshold = 20000000;
#elif defined(__PPC64__)
static int threshold = 34500000;
#endif
static long long counts[1];                   // required for PAPI_stop
static unsigned int outside = 0;
static int count_symbols = 0;
static int profile_shared_lib = 0;

#define MAX_SHAREDLIBS 2000
#define MAX_NAME_LEN 1024 
static int sharedlib  = 0;       // track symbols in shared libs
static int sharedlib_hits[MAX_SHAREDLIBS];
static char sharedlib_name[MAX_SHAREDLIBS][MAX_NAME_LEN];

int index_from_name(const char * name);

static void freq_delay(long *isum)
{
   long lsum = *isum;
   long i, npts = 10000000L;
   for (i = 0L; i < npts; i++) lsum += i;
   *isum = lsum;
}

static void print_papi_error(int);

// This is the overflow interrupt handler for shared libs.
void histoSharedlibHandler(int handle, void * address, long long overflow, void * context)
{
    int rc, indx;
    Dl_info dlinfo;
    long base;

    rc = dladdr(address, &dlinfo);
    if (rc != 0) {
       base = (long) dlinfo.dli_fbase;

#ifdef __x86_64__
       // save each address 
       unsigned long bin = (unsigned long) ((unsigned long) address - base - offset);
#else
       // each address takes 4 bytes so bin = ((address - mystart) >> 2
       unsigned long bin = (unsigned long) (((unsigned long) address - base - offset) >> 2);
#endif

       if (bin >= 0 && bin < histo_size) histogram[bin]++;
       else {
          outside ++;
          if (dlinfo.dli_fname != NULL)  {
             if (dlinfo.dli_fname[0] != '\0') indx = index_from_name(dlinfo.dli_fname);
             else                             indx = index_from_name("unknown");
          }
          else                                indx = index_from_name("unknown");
          sharedlib_hits[indx] ++;
       }
    }
}

// This is the standard overflow interrupt handler.
void histoLibHandler(int handle, void * address, long long overflow, void * context)
{
    int rc, indx;
    Dl_info dlinfo;

#ifdef __x86_64__
    // save each address
    unsigned long bin = (unsigned long) ((unsigned long) address - offset);
#else
    // each address takes 4 bytes so bin = ((address - mystart) >> 2
    unsigned long bin = (unsigned long) (((unsigned long) address - offset) >> 2);
#endif

    if (bin >= 0 && bin < histo_size) histogram[bin]++;
    else {
       outside ++;
       rc = dladdr(address, &dlinfo);
       if (rc != 0) {
          if (dlinfo.dli_fname != NULL)  {
             if (dlinfo.dli_fname[0] != '\0') indx = index_from_name(dlinfo.dli_fname);
             else                             indx = index_from_name("unknown");
          }
          else                                indx = index_from_name("unknown");
          sharedlib_hits[indx] ++;
       }
    }
}


// This is the overflow interrupt handler.
void histoSymHandler(int handle, void * address, long long overflow, void * context)
{
    int rc, indx;
    Dl_info dlinfo;

#ifdef __x86_64__
    // save each address
    unsigned long bin = (unsigned long) ((unsigned long) address - offset);
#else
    // each address takes 4 bytes so bin = ((address - mystart) >> 2
    unsigned long bin = (unsigned long) (((unsigned long) address - offset) >> 2);
#endif

    if (bin >= 0 && bin < histo_size) histogram[bin]++;
    else {
       outside ++;
       rc = dladdr(address, &dlinfo);
       if (rc != 0) {
          if (dlinfo.dli_sname != NULL)  {
             if (dlinfo.dli_sname[0] != '\0') indx = index_from_name(dlinfo.dli_sname);
             else                             indx = index_from_name("unknown");
          }
          else                                indx = index_from_name("unknown");
          sharedlib_hits[indx] ++;
       }
    }
}

// Routine to find the index matching a name
int index_from_name(const char * name)
{
   int i, match;
   char * ptr;

   if (sharedlib < MAX_SHAREDLIBS)
   {
       match = 0;
       for (i=sharedlib-1; i>=0; i--)
       {
           if (0 == strcmp(sharedlib_name[i], name))
           {
               match = 1;
               break;
           }
       }

       if (match == 0)
       {
           i = sharedlib;
           ptr = strcpy(sharedlib_name[i], name);
           if (ptr == NULL) sharedlib_name[i][0] = '\0';
           sharedlib ++;
       }
   }

   return i;
}


// initialization routine
void HPM_Prof_init(void)
{
   int i, myrank, rc;
   size_t nbytes;
   char * ptr, * list_ptr;
   char delimiters[] = {","};
   char envstr[320];
   bfd *file = NULL;
   sec_ptr section;
   pid_t pid;
   char my_exe[512], exe_path[512];
   const char *target = TARGET;
   struct timeval tv1, tv2;
   long isum;
   int freqHandle = PAPI_NULL;
   double freq_Hz, elapsed_seconds;
   long long freq_counter_in, freq_counter_out;
   int interrupt_filter = 1;

#ifdef SERIAL_VERSION
   myrank = 0;
#else
   PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

   // optionally enable interrupts only for specified ranks
   list_ptr = getenv("SAVE_LIST");
   if (list_ptr != NULL)
   {
      memset(envstr, '\0', sizeof(envstr));
      strcpy(envstr, list_ptr);
      interrupt_filter = 0;
      ptr = strtok(envstr, delimiters);
      while(ptr != NULL)
      {
         if (atoi(ptr) == myrank) 
         {
            interrupt_filter = 1;
            break;
         }
         ptr = strtok(NULL, delimiters);
      }
   }

   if (interrupt_filter == 0) return;

   for (i=0; i<MAX_SHAREDLIBS; i++) sharedlib_hits[i] = 0LL;

   memset(sharedlib_name, '\0', MAX_SHAREDLIBS*MAX_NAME_LEN);

   memset(my_exe, '\0', sizeof(my_exe));

   // use BFD methods to locate the program text section; map to histogram
   profile_shared_lib = 0;
   ptr = getenv("PROFILE_SHAREDLIB");
   if (ptr != NULL) {
      strcpy(my_exe, ptr);
      profile_shared_lib = 1;
   }
   else {
      pid = getpid();
      sprintf(exe_path, "/proc/%d/exe", (int) pid);
      rc = readlink(exe_path, my_exe, sizeof(my_exe));
      if (rc == -1) {
         if (myrank == 0) printf("readlink for /proc/pid/exe failed ... returning\n");
         return;
      }
   }

   bfd_init ();

   if (!bfd_set_default_target(target)) {
      if (myrank == 0) printf("bfd_set_default_target failed ... returning\n");
      return;
   }

   file = bfd_openr(my_exe, target);
   if (file == NULL) {
      if (myrank == 0) printf("bfd_openr failed for %s ... returning\n", my_exe);
      return;
   }

   if (!bfd_check_format (file, bfd_object)) {
      if (myrank == 0) printf("problem with executable format ... returning\n");
      return;
   }

   section = bfd_get_section_by_name(file, ".text");
   if (section == NULL) {
      if (myrank == 0) printf("bfd_get_section_by_name failed ... returning\n");
      return;
   }

   histo_size = (long ) section->size;
   offset     = (long ) section->vma;

   // note : histo_size defined here = size of program text in bytes
   if (histo_size < 0L) {
      if (myrank == 0) printf("HPM_Prof_init : buffer size is negative ... returning\n");
      return;
   }

   // this reserves more space than is required
   nbytes = histo_size * sizeof(int);

   histogram = (int *) malloc(nbytes);

   for (i=0; i<histo_size; i++) histogram[i] = 0;

#ifndef CPU_MHZ
   // initialize PAPI to memasure the effective CPU frequency
   rc = PAPI_library_init(PAPI_VER_CURRENT);
   if (rc < 0) {
      if (myrank == 0) fprintf(stderr, "papi initialization failed ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }
  
   rc = PAPI_create_eventset(&freqHandle);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to create an event set\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   rc = PAPI_add_named_event(freqHandle, DEFAULT_EVENT);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to add event ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   rc = PAPI_start(freqHandle);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to start ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   gettimeofday(&tv1, NULL);
   rc = PAPI_read(freqHandle, &freq_counter_in);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI_read failed ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   isum = 0L;
   freq_delay(&isum);

   rc = PAPI_read(freqHandle, &freq_counter_out);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI_read failed ... isum = %lld ... counters not enabled\n", isum);
      if (myrank == 0) print_papi_error(rc);
      return;
   }
   gettimeofday(&tv2, NULL);

   elapsed_seconds = ( ((double) tv2.tv_sec)  - ((double) tv1.tv_sec)  )
            + 1.0e-6*( ((double) tv2.tv_usec) - ((double) tv1.tv_usec) );

   freq_Hz = ((double) (freq_counter_out - freq_counter_in))/elapsed_seconds;

   threshold = (int) (1.0e-2*freq_Hz);

   rc = PAPI_stop(freqHandle, &freq_counter_out);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI_stop failed ... counters not enabled\n", isum);
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   PAPI_shutdown();
#else
   freq_Hz = 1.0e6*CPU_MHZ;
   threshold = (int) (1.0e-2*freq_Hz);
#endif
  
   memset(eventName, '\0', sizeof(eventName));

   // check env variables and enable an event and optional threshold value
   ptr = getenv("HPM_PROFILE_EVENT");
   if (ptr != NULL) {
      strcpy(eventName, ptr);
   }
   else strcpy(eventName, DEFAULT_EVENT);

   ptr = getenv("HPM_PROFILE_THRESHOLD");
   if (ptr != NULL) {
      threshold = atoi(ptr);
   }

   // re-initialize PAPI to start program sampling
   rc = PAPI_library_init(PAPI_VER_CURRENT);
   if (rc < 0) {
      if (myrank == 0) fprintf(stderr, "papi initialization failed ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   rc = PAPI_create_eventset(&eventHandle);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to create an event set ... counters not enabled\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   rc = PAPI_add_named_event(eventHandle, eventName);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to add event %s\n", eventName);
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   rc = PAPI_event_name_to_code(eventName, &eventCode);
   if (rc != PAPI_OK) {
      if (myrank == 0) fprintf(stderr, "PAPI failed to translate the event name\n");
      if (myrank == 0) print_papi_error(rc);
      return;
   }

   count_symbols = 0;

   if (profile_shared_lib) {
      rc = PAPI_overflow(eventHandle, eventCode, threshold, 0, histoSharedlibHandler);
      if (rc != PAPI_OK) {
         if (myrank == 0) fprintf(stderr, "PAPI overflow handler not installed\n");
         if (myrank == 0) print_papi_error(rc);
         return;
      }
   }
   else {
      ptr = getenv("HPM_PROFILE_HANDLER");
      if (ptr != NULL) {
         if (0 == strncasecmp(ptr, "sym", 3)) count_symbols = 1;
         else                                 count_symbols = 0;
      }

      if (count_symbols == 1) {
         rc = PAPI_overflow(eventHandle, eventCode, threshold, 0, histoSymHandler);
         if (rc != PAPI_OK) {
            if (myrank == 0) fprintf(stderr, "PAPI overflow handler not installed\n");
            if (myrank == 0) print_papi_error(rc);
            return;
         }
      }
      else {
         rc = PAPI_overflow(eventHandle, eventCode, threshold, 0, histoLibHandler);
         if (rc != PAPI_OK) {
            if (myrank == 0) fprintf(stderr, "PAPI overflow handler not installed\n");
            if (myrank == 0) print_papi_error(rc);
            return;
         }
      }
   }

   if (myrank == 0) printf("HPM profiling is enabled with event = %s, threshold = %d.\n", eventName, threshold);

   initialized = 1;

}

#ifndef SERIAL_VERSION
// this routine is called from MPI_Init() only, to start counting automatically
void HPM_Prof_Auto_start(void)
{
   if (!initialized) return;
   autostart = 1;
   PAPI_start(eventHandle);
}
#endif

void HPM_Prof_start(void)
{
   int i;
   if (!initialized) return;
   // zero-out any data accumulated since automatic start in MPI_Init()
   if (autostart == 1) {
      autostart = 0;
      for (i=0; i<histo_size; i++) histogram[i] = 0;
      for (i=0; i<MAX_SHAREDLIBS; i++) sharedlib_hits[i] = 0LL;
      memset(sharedlib_name, '\0', MAX_SHAREDLIBS*MAX_NAME_LEN);
      outside = 0;
      sharedlib = 0;
   }

   PAPI_start(eventHandle);
}

void hpm_prof_start(void)
{
   int i;
   if (!initialized) return;
   // zero-out any data accumulated since automatic start in MPI_Init()
   if (autostart == 1) {
      autostart = 0;
      for (i=0; i<histo_size; i++) histogram[i] = 0;
      for (i=0; i<MAX_SHAREDLIBS; i++) sharedlib_hits[i] = 0LL;
      memset(sharedlib_name, '\0', MAX_SHAREDLIBS*MAX_NAME_LEN);
      outside = 0;
      sharedlib = 0;
   }

   PAPI_start(eventHandle);
}


void HPM_Prof_stop(void)
{
   if (!initialized) return;
   PAPI_stop(eventHandle, counts);
}

void hpm_prof_stop(void)
{
   if (!initialized) return;
   PAPI_stop(eventHandle, counts);
}


#ifdef SERIAL_VERSION
void HPM_Prof_write(void)
#else
void HPM_Prof_write(int jobid, int add_timestamp, char * timestamp, int flag)
#endif
{
    int i, myrank, nhits = 0;
    int numaddr = 0;
    FILE * fp;
    int magic, version, len;
    char filename[512];
    char * output_dir = NULL;
#ifdef SERIAL_VERSION
    int jobid, add_timestamp = 0, flag = 1;
    char * timestamp;
    jobid = getpid();
#endif

    if (!initialized) return;

    if (flag == 0) return;

#ifdef SERIAL_VERSION
    myrank = 0;
#else
    PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);
#endif

    for (i=0; i<histo_size; i++) {
       if ( histogram[i] > 0 ) {
          nhits += histogram[i];
          numaddr++;
       }
    }

    if (nhits == 0) {
       if (myrank == 0) fprintf(stderr, "hpmprof recorded no profile hits\n");
       return;
    }

    output_dir = getenv("TRACE_DIR");

    if (add_timestamp)
    {   
       if (output_dir == NULL) sprintf(filename, "hpm_histogram.%d.%s.%d", jobid, timestamp, myrank);
       else                    sprintf(filename, "%s/hpm_histogram.%d.%s.%d", output_dir, jobid, timestamp, myrank);
    }   
    else
    {   
       if (output_dir == NULL) sprintf(filename, "hpm_histogram.%d.%d", jobid, myrank);
       else                    sprintf(filename, "%s/hpm_histogram.%d.%d", output_dir, jobid, myrank);
    }

    fp = fopen(filename, "w");
    if (fp == NULL) {
       printf("failed to open the histogram file ... returning\n");
       return;
    }

    magic = HISTO_MAGIC;
    version = HISTO_VERSION;

    fwrite(&magic,      sizeof(int),       1, fp);
    fwrite(&version,    sizeof(int),       1, fp);
    fwrite(&eventName,  sizeof(eventName), 1, fp);
    fwrite(&threshold,  sizeof(int),       1, fp);
    fwrite(&numaddr,    sizeof(int),       1, fp);
    fwrite(&histo_size, sizeof(int),       1, fp);
    fwrite(&offset,     sizeof(long),      1, fp);
    for (i=0; i<histo_size; i++) {
       if ( histogram[i] > 0 ) {
          fwrite(&i,            sizeof(int), 1, fp);
          fwrite(&histogram[i], sizeof(int), 1, fp);
       }
    }
    fwrite(&outside,    sizeof(int),  1, fp);
    fwrite(&sharedlib,  sizeof(int),  1, fp);
    for (i=0; i<sharedlib; i++) {
       len = 1 + strlen(sharedlib_name[i]);
       fwrite(&sharedlib_hits[i], sizeof(int),  1, fp);
       fwrite(&len, sizeof(int),  1, fp);
       fwrite(sharedlib_name[i], len, 1, fp);
    }
    fclose(fp);

    if (myrank == 0) {
       printf("HPM_Prof got %d hits at %d program-text locations,\n", nhits, numaddr);
       printf("and %d hits at addresses outside the program-text section.\n", outside);
    }

}

void print_papi_error(int rc)
{
   switch(rc) {
     case PAPI_EINVAL :
       fprintf(stderr, "PAPI error : Invalid argument\n");
       break;

     case PAPI_ENOMEM :
       fprintf(stderr, "PAPI error : Insufficient memory\n");
       break;

     case PAPI_ESYS :
       fprintf(stderr, "PAPI error : A System/C library call failed\n");
       break;

     case PAPI_ECMP :
       fprintf(stderr, "PAPI error : Not supported by component\n");
       break;

     case PAPI_ECLOST :
       fprintf(stderr, "PAPI error : Access to the counters was lost or interrupted\n");
       break;

     case PAPI_EBUG :
       fprintf(stderr, "PAPI error : Internal error\n");
       break;

     case PAPI_ENOEVNT :
       fprintf(stderr, "PAPI error : Event does not exist\n");
       break;

     case PAPI_ECNFLCT :
       fprintf(stderr, "PAPI error : Event exists, but cannot be counted due to counter resource limitations\n");
       break;

     case PAPI_ENOTRUN :
       fprintf(stderr, "PAPI error : EventSet is currently not running\n");
       break;

     case PAPI_EISRUN :
       fprintf(stderr, "PAPI error : EventSet is currently counting\n");
       break;

     case PAPI_ENOEVST :
       fprintf(stderr, "PAPI error : No such EventSet available\n");
       break;

     case PAPI_ENOTPRESET :
       fprintf(stderr, "PAPI error : Event in argument is not a valid preset\n");
       break;

     case PAPI_ENOCNTR :
       fprintf(stderr, "PAPI error : Hardware does not support performance counters\n");
       break;

     case PAPI_EMISC :
       fprintf(stderr, "PAPI error : Miscelaneous unknown error code\n");
       break;

     case PAPI_EPERM :
       fprintf(stderr, "PAPI error : Permission level does not permit operation\n");
       break;

     case PAPI_ENOINIT :
       fprintf(stderr, "PAPI error : PAPI hasn't been initialized yet\n");
       break;

     case PAPI_ENOCMP :
       fprintf(stderr, "PAPI error : Component index isn't set\n");
       break;

     case PAPI_ENOSUPP :
       fprintf(stderr, "PAPI error : Not supported\n");
       break;

     case PAPI_ENOIMPL :
       fprintf(stderr, "PAPI error : Not implemented\n");
       break;

     case PAPI_EBUF :
       fprintf(stderr, "PAPI error : Buffer size exceeded\n");
       break;

     case PAPI_EINVAL_DOM :
       fprintf(stderr, "PAPI error : EventSet domain is not supported for the operation\n");
       break;

     case PAPI_EATTR :
       fprintf(stderr, "PAPI error : Invalid or missing event attributes\n");
       break;

     case PAPI_ECOUNT :
       fprintf(stderr, "PAPI error : Too many events or attributes\n");
       break;
 
     case PAPI_ECOMBO :
       fprintf(stderr, "PAPI error : Bad combination of features\n");
       break;

     case PAPI_ECMP_DISABLED :
       fprintf(stderr, "PAPI error : Component containing event is disabled\n");
       break;
   }
}
