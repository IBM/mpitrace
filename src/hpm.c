/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#ifdef USE_MPI
#include <mpi.h>
#endif
#include <papi.h>
#include "counters.h"

// -------------------------------------------------
// C or  C++ routines 
// -------------------------------------------------
// HPM_Init();    for serial jobs
// HPM_Start("label");
// HPM_Stop("label");
// HPM_Reset("label");
// HPM_Enable();
// HPM_Disable();
// HPM_Print();   for serial jobs
// -------------------------------------------------
// Fortran interface to control the sampled region
// -------------------------------------------------
// call hpm_init()   for serial jobs
// call hpm_start('label')
// call hpm_stop('label')
// call hpm_reset('label')
// call hpm_enable();
// call hpm_disable();
// call hpm_print()  for serial jobs
// -------------------------------------------------
#define MAX_CODE_BLOCKS 100
#define LABEL_LEN 96

static void print_papi_error(int);
static int index_from_label(char *);
#ifdef USE_MPI
static void get_ranks_per_node(int *, int *);
static int max_ranks_per_node = 1;
#endif

static int ranks_per_node = 1;
static int local_rank = 0;

#pragma weak hpm_init=HPM_Init
#pragma weak hpm_print=HPM_Print

static long long counter_in[MAX_CODE_BLOCKS][NUM_COUNTERS];
static long long counter_sum[MAX_CODE_BLOCKS][NUM_COUNTERS];
static int block_starts[MAX_CODE_BLOCKS];
static int block_stops[MAX_CODE_BLOCKS];
static double elapsed_in[MAX_CODE_BLOCKS];
static double elapsed_sum[MAX_CODE_BLOCKS];
static char code_block_label[MAX_CODE_BLOCKS][LABEL_LEN];

static int disabled = 0;
static int initialized = 0;
static int code_block = 0;
static int eventHandle = PAPI_NULL;

static int numcounters = NUM_COUNTERS;
static int group = 0;
static int group_list[MAX_GROUPS];
static int use_counter_list = 0;
static int num_groups = 0;
static int collect_all_counts = 1;
static int use_event_list = 0;
static char envname[NUM_COUNTERS][128];

static int world_rank = 0;
static int reader_rank = 0;
static int reader_size = 1;
#ifdef USE_MPI
static MPI_Comm reader_comm = MPI_COMM_WORLD;
#endif

void HPM_Init(void)
{
   int i, j, myrank, nranks, group_is_set, rc, ic;
   char * ptr = NULL;
   char * list_ptr;
   char delimiters[] = {" ,"};
   char counter_name[128];

#ifdef USE_MPI
   PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);
   world_rank = myrank;
   PMPI_Comm_size(MPI_COMM_WORLD, &nranks);
   get_ranks_per_node(&ranks_per_node, &local_rank);
   PMPI_Allreduce(&ranks_per_node, &max_ranks_per_node, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
  #ifdef UNCORE_COUNTERS
   PMPI_Comm_split(MPI_COMM_WORLD, local_rank, myrank, &reader_comm);
   PMPI_Comm_rank(reader_comm, &reader_rank);
   PMPI_Comm_size(reader_comm, &reader_size);
   if (local_rank != 0) return;
  #else  // all ranks read counters
   reader_comm = MPI_COMM_WORLD;
   reader_rank = myrank;
   reader_size = nranks;
  #endif
#else
   myrank = 0;
   nranks = 1;
   local_rank = 0;
   reader_rank = 0;
   reader_size = 1;
#endif

   for (j=0; j<MAX_CODE_BLOCKS; j++) elapsed_sum[j] = 0.0;

   for (j=0; j<MAX_CODE_BLOCKS; j++) {
      block_starts[j] = 0;
      block_stops[j] = 0;
   }

   for (j=0; j<MAX_CODE_BLOCKS; j++) {
      for (ic=0; ic<numcounters; ic++) counter_sum[j][ic] = 0LL;
   }

   // set HPM_GROUP to make all ranks count using the same group
   group_is_set = 0;
   ptr = getenv("HPM_GROUP");
   if (ptr == NULL) group = 0;
   else  {
      group = atoi(ptr);
      if (group < 0 || group >= MAX_GROUPS)  {
         if (myrank == 0) fprintf(stderr, "HPM_GROUP is out of range ... forcing group = 0\n");
         group = 0;
      }
      group_is_set = 1;
   }   

   use_event_list = 0;
   list_ptr = getenv("HPM_EVENT_LIST");
   if ((list_ptr != NULL) && (group_is_set == 0)) {
      use_event_list = 1;
      ptr = strtok(list_ptr, delimiters);
      j = 0;
      while(ptr != NULL) {
         strcpy(envname[j], ptr);
         ptr = strtok(NULL, delimiters);
         j++;
         if (j == NUM_COUNTERS) break;
      }   
      numcounters = j;
      group = 1000;
   }

   // set HPM_GROUP_LIST to make ranks round-robin over a list of groups
   // HPM_GROUP_LIST takes precedence if HPM_GROUP is also set
   list_ptr = getenv("HPM_GROUP_LIST");
   if (list_ptr != NULL) {
      if (strncasecmp(list_ptr,"all",3) == 0) {
         num_groups = MAX_GROUPS;
         for (i=0; i<num_groups; i++) group_list[i] = i;
      }
      else {
         num_groups = 0;
         ptr = strtok(list_ptr, delimiters);
         while(ptr != NULL) {
             group_list[num_groups] = atoi(ptr);
             ptr = strtok(NULL, delimiters);
             num_groups++;
         }

         if ( (num_groups > reader_size) &&  (myrank == 0) )
             fprintf(stderr, "hpm warning: number of groups > number of counter readers ...\n");
      }

      group = group_list[ reader_rank % num_groups ];

      use_counter_list = 1;
   }

   // optionally disable collection of all counts for the job summary
   ptr = getenv("COLLECT_ALL_COUNTS");
   if (ptr != NULL) {
      if (strncasecmp(ptr,"no",2) == 0) collect_all_counts = 0;
      else                              collect_all_counts = 1;
   }

   // for now counting is done by just the master thread
   rc = PAPI_library_init(PAPI_VER_CURRENT);
   if (rc < 0) {
      fprintf(stderr, "PAPI initialization failed for group %d on rank %d ... counters not enabled\n", group, myrank);
      print_papi_error(rc);
      return;
   }

   rc = PAPI_create_eventset(&eventHandle);
   if (rc != PAPI_OK) {
      fprintf(stderr, "PAPI failed to create an event set for group %d on rank %d\n", group, myrank);
      print_papi_error(rc);
      return;
   }

   for (ic = 0; ic<numcounters; ic++) {
      memset(counter_name, '\0', sizeof(counter_name));
      if (use_event_list) sprintf(counter_name, "%s", envname[ic]);
      else                sprintf(counter_name, "%s", CounterGroup[group].event[ic].name);
      rc = PAPI_add_named_event(eventHandle, counter_name);
      if (rc != PAPI_OK) {
         fprintf(stderr, "PAPI failed to add event %s for group %d on rank %d ... counters not enabled\n",  counter_name, group, myrank);
         print_papi_error(rc);
         return;
      }
   }

   rc = PAPI_start(eventHandle);
   if (rc != PAPI_OK) {
      fprintf(stderr, "PAPI failed to start for group %d on rank %d ... counters not enabled\n", group, myrank);
      print_papi_error(rc);
      return;
   }

   if (myrank == 0) {
      if (use_counter_list) printf("HPM counting with a list of groups.\n");
      else                  printf("HPM counting with group = %d.\n", group);
   }

   initialized = 1;

}

void HPM_Disable(void)
{
   disabled = 1;
   return;
}

void HPM_Enable(void)
{
   disabled = 0;
   return;
}

#pragma weak hpm_disable_=hpm_disable
void hpm_disable(void)
{
   disabled = 1;
   return;
}

#pragma weak hpm_enable_=hpm_enable
void hpm_enable(void)
{
   disabled = 0;
   return;
}

void HPM_Reset(char * this_label)
{
   int ic, rc, j;
#ifdef UNCORE_COUNTERS
   if (local_rank != 0) return;
#endif
   j = index_from_label(this_label);

   block_starts[j] = 0;
   block_stops[j] = 0;
   elapsed_sum[j] = 0.0;

   for (ic=0; ic<NUM_COUNTERS; ic++) counter_sum[j][ic] = 0LL;
   
}

#pragma weak hpm_reset_=hpm_reset
void hpm_reset(char * f_label, int length)
{
   int ic, rc, j;
   char this_label[LABEL_LEN];
#ifdef UNCORE_COUNTERS
   if (local_rank != 0) return;
#endif
   strncpy(this_label, f_label, length);
   this_label[length] = '\0';
   j = index_from_label(this_label);

   block_starts[j] = 0;
   block_stops[j] = 0;
   elapsed_sum[j] = 0.0;

   for (ic=0; ic<NUM_COUNTERS; ic++) counter_sum[j][ic] = 0LL;
   
}


void HPM_Start(char * this_label)
{
   int ic, rc, j;
   struct timeval tv;

   if (disabled || !initialized) return;

   gettimeofday(&tv, NULL);

   j = index_from_label(this_label);

   rc = PAPI_read(eventHandle, counter_in[j]);
   if (rc != PAPI_OK) return;

   block_starts[j] += 1;

   elapsed_in[j] = ((double) tv.tv_sec) + 1.0e-6*((double) tv.tv_usec);

   return;

}

#pragma weak hpm_start_=hpm_start
void hpm_start(char * f_label, int length)
{
   int ic, rc, j;
   char this_label[LABEL_LEN];
   struct timeval tv;

   if (disabled || !initialized) return;

   strncpy(this_label, f_label, length);
   this_label[length] = '\0';

   gettimeofday(&tv, NULL);

   j = index_from_label(this_label);

   rc = PAPI_read(eventHandle, counter_in[j]);
   if (rc != PAPI_OK) return;

   block_starts[j] += 1;

   elapsed_in[j] = ((double) tv.tv_sec) + 1.0e-6*((double) tv.tv_usec);

   return;

}



void HPM_Stop(char * this_label)
{
   int ic, rc, j;
   struct timeval tv;
   long long values[NUM_COUNTERS];

   if (disabled || !initialized) return;

   if (code_block >= MAX_CODE_BLOCKS) return;

   gettimeofday(&tv, NULL);

   j = index_from_label(this_label);

   rc = PAPI_read(eventHandle, values);
   if (rc != PAPI_OK) return;

   for (ic=0; ic<numcounters; ic++) counter_sum[j][ic] += (values[ic] - counter_in[j][ic]);

   block_stops[j] += 1;

   elapsed_sum[j] += (((double) tv.tv_sec) + 1.0e-6*((double) tv.tv_usec)) - elapsed_in[j];

   return;
}


#pragma weak hpm_stop_=hpm_stop
void hpm_stop(char * f_label, int length)
{
   int ic, rc, j;
   struct timeval tv;
   long long values[NUM_COUNTERS];
   char this_label[LABEL_LEN];

   if (disabled || !initialized) return;

   if (code_block >= MAX_CODE_BLOCKS) return;

   strncpy(this_label, f_label, length);
   this_label[length] = '\0';

   gettimeofday(&tv, NULL);

   j = index_from_label(this_label);

   rc = PAPI_read(eventHandle, values);
   if (rc != PAPI_OK) return;

   for (ic=0; ic<numcounters; ic++) counter_sum[j][ic] += (values[ic] - counter_in[j][ic]);

   block_stops[j] += 1;

   elapsed_sum[j] += (((double) tv.tv_sec) + 1.0e-6*((double) tv.tv_usec)) - elapsed_in[j];

   return;
}


#ifdef USE_MPI
void HPM_Print(int jobid, int add_timestamp, char * timestamp, int flag)
#else
void HPM_Print(void)
#endif
{
    int j, ic, rank, indx;
    int nblocks, max_nblocks;
    int * all_ranks;
    long long * all_counts;
    double * all_elapsed;
    FILE * fp;
    char filename[256], counter_label[256];
    char * ptr, header[256], counter_string[256];
    struct countStruct {
                         double value;
                         int rank;
                       };
    struct countStruct my_val[NUM_COUNTERS], min_val[NUM_COUNTERS], max_val[NUM_COUNTERS];
    double dble_counts[NUM_COUNTERS], total_counts[NUM_COUNTERS];
    double avg_time, max_time, avg_value;
    int tag = 99;
    int report_rank, report_size, root_rank;
#ifdef USE_MPI
    MPI_Status status;
    MPI_Comm report_comm;
#else
    int jobid;
#endif

    
#ifdef USE_MPI
   #ifdef UNCORE_COUNTERS
    if (local_rank != 0) return;
   #endif
    // if using a list of counter groups, make a communicator for each group
    if (use_counter_list) {
       PMPI_Comm_split(reader_comm, group, reader_rank, &report_comm);
       PMPI_Comm_rank(report_comm, &report_rank);
       PMPI_Comm_size(report_comm, &report_size);
    }
    else {
       report_comm = reader_comm;
       report_rank = reader_rank;
       report_size = reader_size;
    }
#else
    if (!initialized) return;
    report_rank = 0;
    report_size = 1;
#endif

    if (code_block >= MAX_CODE_BLOCKS) nblocks = MAX_CODE_BLOCKS;
    else                               nblocks = code_block;

#ifdef USE_MPI
    // ensure that all ranks in report_comm participate in reductions
    PMPI_Allreduce(&nblocks, &max_nblocks, 1, MPI_INT, MPI_MAX, report_comm);
#else
    ranks_per_node = 1;
    max_nblocks = nblocks;
#endif

#ifdef USE_MPI
    // this section prints a summary of data from all ranks in the reporting group
    if ((report_rank == 0) && (max_nblocks > 0)) {
        if (add_timestamp) sprintf(filename, "hpm_job_summary.%d.%s.%d", jobid, timestamp, group);
        else               sprintf(filename, "hpm_job_summary.%d.%d", jobid, group);
        fp = fopen(filename, "w");
        if (fp == NULL) fp = stderr;

        fprintf(fp, "======================================================================\n");
        fprintf(fp, "Hardware counter summary for job %d, counter group %d.\n", jobid, group);
        fprintf(fp, "Number of MPI ranks in the reporting group = %d.\n", report_size);
        fprintf(fp, "Max number of MPI ranks per node = %d.\n", max_ranks_per_node);
        fprintf(fp, "======================================================================\n");
      
    }

    for (j=0; j<max_nblocks; j++) {
       if (block_starts[j] == block_stops[j]) {

          for (ic=0; ic<numcounters; ic++) {
             dble_counts[ic]  = (double) counter_sum[j][ic];
             my_val[ic].value = (double) counter_sum[j][ic];
             my_val[ic].rank  = world_rank;
          }

          PMPI_Allreduce(dble_counts, total_counts, numcounters, MPI_DOUBLE, MPI_SUM, report_comm);
          PMPI_Allreduce(my_val, min_val, numcounters, MPI_DOUBLE_INT, MPI_MINLOC, report_comm);
          PMPI_Allreduce(my_val, max_val, numcounters, MPI_DOUBLE_INT, MPI_MAXLOC, report_comm);

          PMPI_Allreduce(&elapsed_sum[j], &avg_time, 1, MPI_DOUBLE, MPI_SUM, report_comm);
          PMPI_Allreduce(&elapsed_sum[j], &max_time, 1, MPI_DOUBLE, MPI_MAX, report_comm);
          avg_time = avg_time/((double) report_size);

          if (report_rank == 0) {
             fprintf(fp, "----------------------------------------------------------------\n");
             fprintf(fp, "%s, call count = %d, avg time = %.4lf, max time = %.4lf :\n",
                     code_block_label[j], block_starts[j], avg_time, max_time);
             fprintf(fp, "  -- Counter values for processes in this reporting group ----\n");
             fprintf(fp, "    min-value  min-rank    max-value  max-rank    avg-value   label\n");
             for (ic=0; ic<numcounters; ic++) {
                memset(counter_label, '\0', sizeof(counter_label));
                if (use_event_list) sprintf(counter_label, "%s", envname[ic]);
                else                sprintf(counter_label, "(%s) %s", CounterGroup[group].event[ic].name, CounterGroup[group].event[ic].label);
                avg_value = total_counts[ic]/((double) report_size);
                fprintf(fp, "%14.6e %7d %14.6e %7d %14.6e %s\n", min_val[ic].value, min_val[ic].rank,
                             max_val[ic].value, max_val[ic].rank, avg_value, counter_label);
             }
             fprintf(fp, "\n");
          }

          if (collect_all_counts == 1) {
             if (report_rank == 0)  {
                all_counts = (long long *) malloc(report_size*numcounters*sizeof(long long));
                all_ranks = (int *)        malloc(report_size*sizeof(int));
                all_elapsed = (double *) malloc(report_size*sizeof(double));
             }
             else  {
                all_counts = NULL;
                all_ranks  = NULL;
                all_elapsed = NULL;
             }
             PMPI_Gather(counter_sum[j], numcounters, MPI_LONG_LONG, all_counts, numcounters, MPI_LONG_LONG, 0, report_comm);
             PMPI_Gather(&world_rank, 1, MPI_INT, all_ranks, 1, MPI_INT, 0, report_comm);
             PMPI_Gather(&elapsed_sum[j], 1, MPI_DOUBLE, all_elapsed, 1, MPI_DOUBLE, 0, report_comm);
             if (report_rank == 0) {
                // create a header for the list of counter values; assume numcounters <= 10
                ptr = counter_string;
                for (ic = 0; ic < numcounters; ic++) {
                   sprintf(ptr, "%15s%d", "counter", ic + 1); 
                   ptr += 16; 
                }
                sprintf(header, "world_rank   elapsed(sec)");
                strcat(header, counter_string);
                fprintf(fp, "%s\n", header);
                for (rank=0; rank<report_size; rank++) {
                   indx = rank*numcounters;
                   fprintf(fp, "%-10d   %12.3lf  ", all_ranks[rank], all_elapsed[rank]);
                   for (ic=0; ic<numcounters; ic++) fprintf(fp, "%14lld  ", all_counts[indx+ic]);
                   fprintf(fp, "\n");
                }
             }
          }
       }
       else {
          if (report_rank == 0) {
             fprintf(fp, "mismatch in starts/stops for code block '%s'\n", code_block_label[j]);
             fprintf(fp, "  starts = %d\n", block_starts[j]);
             fprintf(fp, "  stops  = %d\n", block_stops[j]);
             fprintf(fp, "\n");
          }
       }
    }

    if ((report_rank == 0 ) && (max_nblocks > 0)  &&  (fp != stderr)) fclose(fp);

    // save at least one process summary file for each counter group in the list
    if (use_counter_list) {
       if (reader_rank < num_groups) flag = 1;
    }

    if (flag == 0 || (!initialized)) return;
#endif

    // the following section has counts from ranks that have output filter enabled
#ifdef USE_MPI
    // skip the per-process files when using collect_all_counts
    if (collect_all_counts) return;  
    if (add_timestamp) sprintf(filename, "hpm_process_summary.%d.%s.%d", jobid, timestamp, world_rank);
    else               sprintf(filename, "hpm_process_summary.%d.%d", jobid, world_rank);

#else
    jobid = getpid();
    sprintf(filename, "hpm_group%d.%d.txt", group,jobid);
#endif

    fp = fopen(filename, "w");
    if (fp == NULL) fp = stderr;

#ifdef USE_MPI
    fprintf(fp, "======================================================================\n");
    fprintf(fp, "Hardware counter data, group = %d, rank = %d.\n", group, world_rank);
    fprintf(fp, "Number of MPI ranks per node = %d.\n", ranks_per_node);
    fprintf(fp, "======================================================================\n");
#else
    fprintf(fp, "======================================================================\n");
    fprintf(fp, "Hardware counter data, group = %d.\n", group);
    fprintf(fp, "======================================================================\n");
#endif

    for (j=0; j<nblocks; j++) {
       if (block_starts[j] == block_stops[j]) {
          fprintf(fp, "----------------------------------------------------------------\n");
          fprintf(fp, "%s, call count = %d, elapsed time = %.4lf\n", code_block_label[j], block_starts[j], elapsed_sum[j]);
          for (ic=0; ic<numcounters; ic++) {
             memset(counter_label, '\0', sizeof(counter_label));
             if (use_event_list) sprintf(counter_label, "%s", envname[ic]);
             else                sprintf(counter_label, "(%s) %s", CounterGroup[group].event[ic].name, CounterGroup[group].event[ic].label);
             fprintf(fp, "%15lld : %s\n", counter_sum[j][ic], counter_label);
          }
       }
       else {
          fprintf(fp, "mismatch in starts/stops for code block '%s'\n", code_block_label[j]);
          fprintf(fp, "  starts = %d\n", block_starts[j]);
          fprintf(fp, "  stops  = %d\n", block_stops[j]);

       }
       fprintf(fp, "\n");
    }

    if (fp != stderr) fclose(fp);
}

#ifdef USE_MPI
void HPM_Print_myrank(int jobid, int add_timestamp, char * timestamp, int flag)
{
    int j, ic, rank;
    int nblocks, max_nblocks;
    FILE * fp;
    char filename[256], counter_label[256];
    struct countStruct {
                         double value;
                         int rank;
                       };
    double dble_counts[NUM_COUNTERS], total_counts[NUM_COUNTERS];
    double avg_time, max_time, avg_value;
    int tag = 99;
    int report_rank, report_size, root_rank;

    if (!initialized) return;

    if (!flag) return;

    if (code_block >= MAX_CODE_BLOCKS) nblocks = MAX_CODE_BLOCKS;
    else                               nblocks = code_block;

    max_nblocks = nblocks;

    if (add_timestamp) sprintf(filename, "hpm_process_summary.%d.%s.%d", jobid, timestamp, world_rank);
    else               sprintf(filename, "hpm_process_summary.%d.%d", jobid, world_rank);
    fp = fopen(filename, "w");
    if (fp == NULL) fp = stderr;

    fprintf(fp, "======================================================================\n");
    fprintf(fp, "Hardware counter data, group = %d, rank = %d.\n", group, world_rank);
    fprintf(fp, "======================================================================\n");

    for (j=0; j<nblocks; j++) {
       if (block_starts[j] == block_stops[j]) {
          fprintf(fp, "----------------------------------------------------------------\n");
          fprintf(fp, "%s, call count = %d, elapsed time = %.4lf\n", code_block_label[j], block_starts[j], elapsed_sum[j]);
          for (ic=0; ic<numcounters; ic++) {
             memset(counter_label, '\0', sizeof(counter_label));
             if (use_event_list) sprintf(counter_label, "%s", envname[ic]);
             else                sprintf(counter_label, "(%s) %s", CounterGroup[group].event[ic].name, CounterGroup[group].event[ic].label);
             fprintf(fp, "%15lld : %s\n", counter_sum[j][ic], counter_label);
          }
       }
       else {
          fprintf(fp, "mismatch in starts/stops for code block '%s'\n", code_block_label[j]);
          fprintf(fp, "  starts = %d\n", block_starts[j]);
          fprintf(fp, "  stops  = %d\n", block_stops[j]);

       }
       fprintf(fp, "\n");
    }

    if (fp != stderr) fclose(fp);
}
#endif


static int index_from_label(char * this_label)
{
   int i, match;
   char * ptr;

   if (code_block < MAX_CODE_BLOCKS)
   {
       match = 0;
       for (i=code_block-1; i>=0; i--)
       {
           if (0 == strcmp(code_block_label[i], this_label))
           {
               match = 1;
               break;
           }
       }

       if (match == 0)
       {
           i = code_block;
           ptr = strcpy(code_block_label[i], this_label);
           if (ptr == NULL) code_block_label[i][0] = '\0';
           code_block ++;
       }
   }

   return i;

}


#ifdef USE_MPI
static void get_ranks_per_node(int * ppn, int * lrank)
{
  int i, rc;
  int nranks, myrank, color, match;
  char host[80];
  char * ptr, * snames, * rnames;
  MPI_Comm local_comm;

  rc = PMPI_Comm_size(MPI_COMM_WORLD, &nranks);
  rc = PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);

  snames = (char *) malloc(nranks*sizeof(host));
  rnames = (char *) malloc(nranks*sizeof(host));
  gethostname(host, sizeof(host));

  for (i=0; i<sizeof(host); i++) {
     if (host[i] == '.') {
         host[i] = '\0';
         break;
     }
  }

  for (i=0; i<nranks; i++)  {
    ptr = snames + i*sizeof(host);
    strncpy(ptr, host, sizeof(host));
  }

  MPI_Alltoall(snames, sizeof(host), MPI_BYTE,
               rnames, sizeof(host), MPI_BYTE, MPI_COMM_WORLD);
  color = 0;
  match = 0;
  for (i=0; i<nranks; i++) {
    ptr = rnames + i*sizeof(host);
    if (strcmp(host, ptr) == 0) {
      match++;
      if (match == 1) color = i;
    }
  }

  MPI_Comm_split(MPI_COMM_WORLD, color, myrank, &local_comm);
  MPI_Comm_rank(local_comm, lrank);
  MPI_Comm_size(local_comm, ppn);

  return;

}
#endif

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
