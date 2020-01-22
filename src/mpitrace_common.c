/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

/*----------------------------------------------------------*/
/*    Function to log events                                */
/*----------------------------------------------------------*/
void LogEvent(int id, struct timeval TV1, struct timeval TV2,
              int src, int dest, int bytes, MPI_Comm comm)
{
   unsigned int k, bin, limit;
   int size, parent, grandparent;
   int dx1, dy1, dz1;
   int dx2, dy2, dz2;
   int dx, dy, dz;
   double timediff;
   double tbeg, tend;
   MPI_Group group;
   int world_dest;
   int write_flag;
   static int first_window_event = 1;
   static int last_window_event = 1;
#ifdef USE_LOCKS
   pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

   if (collect_summary == 0) return;

   tbeg = TCONV(TV1);
   tend = TCONV(TV2);

   if (time_window_profiling) 
   {
      if ( (tend - mpi_init_time) < profile_begin_time ) return;

      if ( (tend - mpi_init_time) > profile_begin_time )
      {
#ifdef USE_LOCKS
          pthread_mutex_lock(&lock);
#endif
          if (first_window_event) 
          {
             mpitrace_summary_start();
#ifdef USE_NVTX
             cudaProfilerStart();
#endif
#ifdef HPM
             HPM_Start("window");
#endif
#ifdef HPMPROF
             HPM_Prof_start();
#endif
             first_window_event = 0;
          }
#ifdef USE_LOCKS
          pthread_mutex_unlock(&lock);
#endif
       }

      if ( (tbeg - mpi_init_time) > profile_end_time )
      {
#ifdef USE_LOCKS
          pthread_mutex_lock(&lock);
#endif
          if (last_window_event)
          {
              last_window_event = 0;
              mpitrace_summary_stop();
#ifdef USE_NVTX
              cudaProfilerStop();
#endif
              write_profile_data_myrank();
#ifdef HPM
             HPM_Stop("window");
             HPM_Print_myrank(jobid, add_timestamp, timestamp, output_filter);
#endif
#ifdef HPMPROF
             HPM_Prof_stop();
             HPM_Prof_write(jobid, add_timestamp, timestamp, output_filter);
#endif
          }
#ifdef USE_LOCKS
          pthread_mutex_unlock(&lock);
#endif

          return;
      }
   }

   timediff = tend - tbeg;

   if (profile_by_call_site)
   {
      get_parents(traceback_level, &parent, &grandparent);

      if (parent != 0)
      {
#ifdef USE_LOCKS
         pthread_mutex_lock(&lock);
#endif
         k = index_from_address(parent);
         profile_call_count[k][id]++;
         profile_callsite_time[k][id] += timediff;
         profile_elapsed_time[k] += timediff;
#ifdef USE_LOCKS
         pthread_mutex_unlock(&lock);
#endif
      }

   }

   if (time_window_tracing)
   {
#ifdef USE_LOCKS
      pthread_mutex_lock(&lock);
#endif
      if ( ((tend - mpi_init_time) > trace_begin_time) && 
           ((tbeg - mpi_init_time) < trace_end_time)      ) trace_events = 1;
      else                                                  trace_events = 0;
#ifdef USE_LOCKS
      pthread_mutex_unlock(&lock);
#endif
   }

   if (trace_events && (disable_tracing[id] == 0) && (event_number < max_events))
   {
#ifdef USE_LOCKS
      pthread_mutex_lock(&lock);
#endif
      get_parents(traceback_level, &parent, &grandparent);
      event[event_number].tbeg          = tbeg - mpi_init_time;
      event[event_number].tend          = tend - mpi_init_time;
      event[event_number].UN.ranks.src  = src;
      event[event_number].UN.ranks.dest = dest;
      event[event_number].taskid        = taskid;
      event[event_number].eventid       = id;
      event[event_number].bytes         = bytes;
      event[event_number].parent        = parent;
      event[event_number].grandparent   = grandparent;
      event[event_number].ioflag        = 0;
      event_number++;
      if (event_number == max_events) event_buffer_overflow = 1;
#ifdef USE_LOCKS
      pthread_mutex_unlock(&lock);
#endif
   }

   /*---------------------------------------------------*/
   /* put messages into bins, at power-of-two intervals */
   /*---------------------------------------------------*/
#ifdef USE_LOCKS
   pthread_mutex_lock(&lock);
#endif
   if (bytes == 0)
   {
      bin = 0; 
      bin_count[id][bin] ++;
      bin_bytes[id][bin] += (double) bytes;
      bin_time[id][bin] += timediff;
   }

   if (bytes > 0)
   {
      bin = 1; limit = 4;
      while (bytes > limit) 
      {
         limit <<= 1;
         bin ++;
      }
      bin_count[id][bin] ++;
      bin_bytes[id][bin] += (double) bytes;
      bin_time[id][bin]  += timediff;
   }

   event_count[id] ++;
   total_time[id ] += timediff;
   if (bytes >= 0) total_bytes[id] += (double) bytes;

   /*----------------------------------------------*/
   /* optionally collect data by communicator size */
   /*----------------------------------------------*/
   if ( comm_profile && (comm != MPI_COMM_NULL) )
   {
      PMPI_Comm_size(comm, &size);
      bin = 0; limit = 4;
      while (size > limit) 
      {
         limit <<= 1;
         bin ++;
      }
      comm_count[id][bin] ++;
      comm_bytes[id][bin] += (double) bytes;
      comm_time[id][bin]  += timediff;
      total_size[bin]     += (double) size;
   }
#ifdef USE_LOCKS
   pthread_mutex_unlock(&lock);
#endif

   /*-----------------------------------------------*/
   /* more diagnostics for message-passing routines */
   /*-----------------------------------------------*/
   if (trace_send_pattern && (dest >= 0))
   {
      /*---------------------------------------------------*/
      /* translate the destination to a rank in comm_world */
      /*---------------------------------------------------*/
#ifdef USE_LOCKS
      pthread_mutex_lock(&lock);
#endif
      if ( (comm != MPI_COMM_WORLD) && translate_ranks )
      {
          MPI_Comm_group(comm, &group);
          MPI_Group_translate_ranks(group, 1, &dest, world_group, &world_dest);
          if (world_dest < ntasks) dest = world_dest;
      }

      send_bytes[dest] += (float) bytes;
      send_count[dest]++;
#ifdef USE_LOCKS
      pthread_mutex_unlock(&lock);
#endif
   }

   return;
}

/*----------------------------------------------------------*/
/*    Function to log IO events                             */
/*----------------------------------------------------------*/
void LogIOEvent(int id, struct timeval TV1, struct timeval TV2,
                long offset, int bytes, MPI_Comm comm)
{
   unsigned int k, bin, limit;
   int size, parent, grandparent;
   int dx1, dy1, dz1;
   int dx2, dy2, dz2;
   int dx, dy, dz;
   double timediff;
   double tbeg, tend;
   int write_flag;
   static int first_window_event = 1;
   static int last_window_event = 1;
#ifdef USE_LOCKS
   pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
#endif

   if (collect_summary == 0) return;

   tbeg = TCONV(TV1);
   tend = TCONV(TV2);

   if (time_window_profiling) 
   {
      if ( (tend - mpi_init_time) < profile_begin_time ) return;

      if ( (tend - mpi_init_time) > profile_begin_time )
      {
#ifdef USE_LOCKS
          pthread_mutex_lock(&lock);
#endif
          if (first_window_event) 
          {
             mpitrace_summary_start();
#ifdef USE_NVTX
             cudaProfilerStart();
#endif
#ifdef HPM
             HPM_Start("window");
#endif
#ifdef HPMPROF
             HPM_Prof_start();
#endif
             first_window_event = 0;
          }
#ifdef USE_LOCKS
          pthread_mutex_unlock(&lock);
#endif
       }

      if ( (tbeg - mpi_init_time) > profile_end_time )
      {
#ifdef USE_LOCKS
          pthread_mutex_lock(&lock);
#endif
          if (last_window_event)
          {
              last_window_event = 0;
              mpitrace_summary_stop();
#ifdef USE_NVTX
              cudaProfilerStop();
#endif
              write_profile_data_myrank();
#ifdef HPM
             HPM_Stop("window");
             HPM_Print_myrank(jobid, add_timestamp, timestamp, output_filter);
#endif
#ifdef HPMPROF
             HPM_Prof_stop();
             HPM_Prof_write(jobid, add_timestamp, timestamp, output_filter);
#endif
          }
#ifdef USE_LOCKS
          pthread_mutex_unlock(&lock);
#endif

          return;
      }
   }

   timediff = tend - tbeg;

   if (profile_by_call_site)
   {
      get_parents(traceback_level, &parent, &grandparent);

      if (parent != 0)
      {
#ifdef USE_LOCKS
         pthread_mutex_lock(&lock);
#endif
         k = index_from_address(parent);
         profile_call_count[k][id]++;
         profile_callsite_time[k][id] += timediff;
         profile_elapsed_time[k] += timediff;
#ifdef USE_LOCKS
         pthread_mutex_unlock(&lock);
#endif
      }

   }

   if (time_window_tracing)
   {
#ifdef USE_LOCKS
      pthread_mutex_lock(&lock);
#endif
      if ( ((tend - mpi_init_time) > trace_begin_time) && 
           ((tbeg - mpi_init_time) < trace_end_time)      ) trace_events = 1;
      else                                                  trace_events = 0;
#ifdef USE_LOCKS
      pthread_mutex_unlock(&lock);
#endif
   }

   if (trace_events && (disable_tracing[id] == 0) && (event_number < max_events))
   {
#ifdef USE_LOCKS
      pthread_mutex_lock(&lock);
#endif
      get_parents(traceback_level, &parent, &grandparent);
      event[event_number].tbeg        = tbeg - mpi_init_time;
      event[event_number].tend        = tend - mpi_init_time;
      event[event_number].UN.offset   = offset;
      event[event_number].taskid      = taskid;
      event[event_number].eventid     = id;
      event[event_number].bytes       = bytes;
      event[event_number].parent      = parent;
      event[event_number].grandparent = grandparent;
      event[event_number].ioflag      = 1;
      event_number++;
      if (event_number == max_events) event_buffer_overflow = 1;
#ifdef USE_LOCKS
      pthread_mutex_unlock(&lock);
#endif
   }

   /*---------------------------------------------------*/
   /* put messages into bins, at power-of-two intervals */
   /*---------------------------------------------------*/
#ifdef USE_LOCKS
   pthread_mutex_lock(&lock);
#endif
   if (bytes == 0)
   {
      bin = 0; 
      bin_count[id][bin] ++;
      bin_bytes[id][bin] += (double) bytes;
      bin_time[id][bin] += timediff;
   }

   if (bytes > 0)
   {
      bin = 1; limit = 4;
      while (bytes > limit) 
      {
         limit <<= 1;
         bin ++;
      }
      bin_count[id][bin] ++;
      bin_bytes[id][bin] += (double) bytes;
      bin_time[id][bin]  += timediff;
   }

   event_count[id] ++;
   total_time[id ] += timediff;
   if (bytes >= 0) total_bytes[id] += (double) bytes;

   /*----------------------------------------------*/
   /* optionally collect data by communicator size */
   /*----------------------------------------------*/
   if ( comm_profile && (comm != MPI_COMM_NULL) )
   {
      PMPI_Comm_size(comm, &size);
      bin = 0; limit = 4;
      while (size > limit) 
      {
         limit <<= 1;
         bin ++;
      }
      comm_count[id][bin] ++;
      comm_bytes[id][bin] += (double) bytes;
      comm_time[id][bin]  += timediff;
      total_size[bin]     += (double) size;
   }
#ifdef USE_LOCKS
   pthread_mutex_unlock(&lock);
#endif

   return;
}

#define MAX_CALL_DEPTH 100
/*===========================================================*/
/* routine to get the parent/grandparent instruction address */
/*===========================================================*/
void get_parents(int level, int *parent, int *grandparent)
{
  int i, depth;
  void * addresses[MAX_CALL_DEPTH];
#ifdef BIG_ENDIAN
  struct uPair { unsigned hi ; unsigned lo ; };
#else
  struct uPair { unsigned lo ; unsigned hi ; };
#endif
  union { long longval; struct uPair intpair; } Addr64;

  depth = backtrace(addresses, MAX_CALL_DEPTH);

  /*----------------------------------------------------*/
  /* Note: 1 = LogEvent, 2 = MPI wrapper, 3 = user code */
  /*----------------------------------------------------*/
  Addr64.longval = (long) addresses[3+level];
  if (depth - 5 > level) *parent = Addr64.intpair.hi;
  else                   *parent = 0;

  Addr64.longval = (long) addresses[4+level];
  if (depth - 6 > level) *grandparent = Addr64.intpair.hi;
  else                   *grandparent = 0;

  return;
}

/*----------------------------------*/
/* mpi error handler with traceback */
/*----------------------------------*/
void mpitrace_handler(MPI_Comm *comm, int *error)
{
   int rank, len, info = 0;
   char string[256];

   MPI_Error_string(*error, string, &len);
   MPI_Comm_rank(*comm, &rank);
   fprintf(stderr, "MPI rank %d : %s\n", rank, string);
   mpitrace_traceback(&rank);
   MPI_Abort(MPI_COMM_WORLD, info);
}


/*-------------------------------------------------------*/
/* use -g and addr2line to get the file and line number. */
/*-------------------------------------------------------*/
void mpitrace_traceback(int *rank)
{
  int level;
  void * addresses[MAX_CALL_DEPTH];
  int depth;
  int i;

  depth = backtrace(addresses, MAX_CALL_DEPTH);

  fprintf(stderr,"MPI rank %d : traceback instruction addresses :\n", *rank);
  for (level=2; level<depth-1; level++)
  {
     fprintf(stderr, "MPI rank %d :  level %d => %#10.8lx\n",
             *rank, level-1, (unsigned long int) addresses[level]);
  }
  fprintf(stderr,"MPI rank %d :\n", *rank);

  return;
}


/*===========================================================*/
/* routine to write a trace file with optional byte-swap     */
/*===========================================================*/
static void write_tracefile(FILE * fd, struct eventstruct * ev, int nbytes)
{
   int rc, chunk, nchunks, leftover;
   int chunksize;
   char swapped[32768]; // must be adequate for 400 records
   char * buffer;


   if (swap_bytes)
   {
     chunksize = 400*sizeof(struct eventstruct);
     nchunks = nbytes/chunksize;
     leftover = nbytes - nchunks*chunksize;

     buffer = (char *) ev;
     for (chunk=0; chunk<nchunks; chunk++)
     {
        reverse_byte_order(buffer, swapped, chunksize);
        rc = fwrite(swapped, 1, chunksize, fd);
        if (rc != chunksize) perror("write_tracefile");
        buffer += chunksize;
     }

     if (leftover != 0)
     {
        reverse_byte_order(buffer, swapped, leftover);
        rc = fwrite(swapped, 1, leftover, fd);
        if (rc != leftover) perror("write_tracefile");
     }
   }

   else
   {
     rc = fwrite(ev, 1, nbytes, fd);
     if (rc != nbytes) perror("write_tracefile");
   }
}


/*===========================================================*/
/* routine to start logging MPI events to a trace file       */
/*===========================================================*/
void mpitrace_trace_start(void)
{
    trace_events = 1;

    if (first_trace_start)
    {
        event_number = 0;
        first_trace_start = 0;
#ifdef USE_NVTX
        cudaProfilerStart();
#endif
    }
}

void mpitrace_trace_start_(void)
{
    trace_events = 1;

    if (first_trace_start)
    {
        event_number = 0;
        first_trace_start = 0;
#ifdef USE_NVTX
        cudaProfilerStart();
#endif
    }
}


/*===========================================================*/
/* routine to stop logging MPI events to a trace file        */
/*===========================================================*/
void mpitrace_trace_stop(void)
{
    trace_events = 0;
#ifdef USE_NVTX
    cudaProfilerStop();
#endif
}

void mpitrace_trace_stop_(void)
{
    trace_events = 0;
#ifdef USE_NVTX
    cudaProfilerStop();
#endif
}


/*===========================================================*/
/* routine to start collecting MPI summary data              */
/*===========================================================*/
void mpitrace_summary_start(void)
{
    struct timeval TV;

    summary_start_count++;

    gettimeofday(&TV, NULL);

    exclusive_time_start = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

    collect_summary = 1;

    if (first_summary_start)
    {
       first_summary_start = 0;

       initialize_summary_data();
    }

}

void mpitrace_summary_start_(void)
{
    struct timeval TV;

    summary_start_count++;

    gettimeofday(&TV, NULL);

    exclusive_time_start = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

    collect_summary = 1;

    if (first_summary_start)
    {
       first_summary_start = 0;

       initialize_summary_data();
    }

}


/*===========================================================*/
/* routine to stop collecting MPI cumulative data            */
/*===========================================================*/
void mpitrace_summary_stop(void)
{
    if (collect_summary == 1) stop_timers();

    collect_summary = 0;
}

void mpitrace_summary_stop_(void)
{
    if (collect_summary == 1) stop_timers();

    collect_summary = 0;
}


/*==========================================================*/
/* routine to reset initial times and cumulative statistics */
/*==========================================================*/
void initialize_summary_data(void)
{
    int i, id, bin;
    struct timeval TV;
    struct rusage RU;

    for (id=0; id<MAX_IDS; id++)
    {
       event_count[id] = 0LL;
       total_time[id]  = 0.0;
       total_bytes[id] = 0.0;

       for (bin=0; bin<MAX_BINS; bin++)
       {
          bin_count[id][bin] = 0LL;
          bin_bytes[id][bin] = 0.0;
          bin_time[id][bin]  = 0.0;
       }
    }

    if (comm_profile)
    {
       for (id=0; id<MAX_IDS; id++)
       {
          for (bin=0; bin<MAX_BINS; bin++)
          {
             comm_count[id][bin] = 0LL;
             comm_bytes[id][bin] = 0.0;
             comm_time[id][bin]  = 0.0;
          }
       }

       for (bin=0; bin<MAX_BINS; bin++) total_size[bin] = 0;
    }

    if (profile_by_call_site) 
    {
       for (i=0; i<MAX_PROFILE_BLOCKS; i++)
       {
           profile_elapsed_time[i] = 0.0;

           for (id=0; id<MAX_IDS; id++)
           {
               profile_call_count[i][id] = 0LL;
               profile_callsite_time[i][id] = 0.0;
           }
       }
    }

    if (trace_send_pattern) 
    {
       for(i=0; i<ntasks; i++) 
       {
          send_bytes[i] = 0.0f;
          send_count[i] = 0;
       }
    }

    if (collective_barrier) synctime = 0.0;
   
    getrusage(RUSAGE_SELF, &RU);

    user_time_initial = (double) (RU.ru_utime).tv_sec
              + 1.0e-6*((double) (RU.ru_utime).tv_usec);

    system_time_initial = (double) (RU.ru_stime).tv_sec
                + 1.0e-6*((double) (RU.ru_stime).tv_usec);

//  context_switches_initial = RU.ru_nvcsw + RU.ru_nivcsw;
    context_switches_initial = RU.ru_nivcsw;

    gettimeofday(&TV, NULL);

    elapsed_time_initial= (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

}


/*==================================================*/
/* routine to stop timers for cumulative statistics */
/*==================================================*/
static void stop_timers(void)
{
    struct timeval TV;
    struct rusage RU;
    double current_time;

    gettimeofday(&TV, NULL);

    current_time = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

    getrusage(RUSAGE_SELF, &RU);

    user_time = (double) (RU.ru_utime).tv_sec
              + 1.0e-6*((double) (RU.ru_utime).tv_usec)
              - user_time_initial;

    system_time = (double) (RU.ru_stime).tv_sec
                + 1.0e-6*((double) (RU.ru_stime).tv_usec)
                - system_time_initial;

    max_memory = ((double) RU.ru_maxrss)/1024.0;

//  context_switches = RU.ru_nvcsw + RU.ru_nivcsw - context_switches_initial;
    context_switches = RU.ru_nivcsw - context_switches_initial;

    elapsed_time = current_time - elapsed_time_initial;

    exclusive_time += (current_time - exclusive_time_start);

}


/*===========================================================*/
/* routine to reverse the byte order in each event record    */
/*===========================================================*/
void reverse_byte_order(char * in, char * out, int num_bytes)
{
   int i;

   for (i=0; i<num_bytes; i+=sizeof(struct eventstruct))
   {
       swap8(&in[i],    &out[i]);     /* 8-bytes for tbeg                */
       swap8(&in[i+8],  &out[i+8]);   /* 8-bytes for tend                */
       swap4(&in[i+16], &out[i+16]);  /* 4-bytes for taskid              */
       swap4(&in[i+20], &out[i+20]);  /* 4-bytes for eventid             */
       swap4(&in[i+24], &out[i+24]);  /* 4-bytes for src                 */
       swap4(&in[i+28], &out[i+28]);  /* 4-bytes for dest                */
       swap4(&in[i+32], &out[i+32]);  /* 4-bytes for bytes               */
       swap4(&in[i+36], &out[i+36]);  /* 4-bytes for parent address      */
       swap4(&in[i+40], &out[i+40]);  /* 4-bytes for grandparent address */
       swap4(&in[i+44], &out[i+44]);  /* 4-bytes for padding             */
   }
}

void swap8(char * in, char * out)
{
   out[7] = in[0];
   out[6] = in[1];
   out[5] = in[2];
   out[4] = in[3];
   out[3] = in[4];
   out[2] = in[5];
   out[1] = in[6];
   out[0] = in[7];
}

void swap4(char * in, char * out)
{
   out[3] = in[0];
   out[2] = in[1];
   out[1] = in[2];
   out[0] = in[3];
}


/*=============================================================*/
/* routine to take an instruction address and return its index */
/*=============================================================*/
int index_from_address(int address)
{
   int i, k, match;

   match = 0;

   /*----------------------------*/
   /* first check the fifo queue */
   /*----------------------------*/
   if (profile_block > FIFO_DEPTH)
   {
      for (k=0; k<FIFO_DEPTH; k++)
      {
          i = profile_fifo[k];
          if (i >= 0)
          {
              if (address == profile_callsite[i])
              {
                  match = 1;
                  break;
              }
          }
      }
   }

   if (match == 1) return i;

   /*-------------------------------------------------------*/
   /* not found in the fifo, so check all known code blocks */
   /*-------------------------------------------------------*/
   for (i=0; i<profile_block; i++)
   {
       if (address == profile_callsite[i])
       {
           match = 1;
           break;
       }
   }

   /*------------------------------------------------*/
   /* if there is no match, this is a new code block */
   /*------------------------------------------------*/
   if (match == 0)
   {
       i = profile_block;
       profile_callsite[i] = address;
       profile_block ++;
   }

   /*-----------------------------------------------*/
   /* save the latest code block in the fifo        */
   /*-----------------------------------------------*/
   for (k=FIFO_DEPTH-2; k>=0; k--) profile_fifo[k+1] = profile_fifo[k];

   profile_fifo[0] = i;

   return i;

}

/*==========================================================*/
/* routine to print the profile with instruction addresses  */
/*==========================================================*/
static void print_profile_by_call_site(FILE * fh)
{
    int i, id, k;
    int * profile_sorted_index;

    profile_sorted_index = (int *) malloc(profile_block*sizeof(int));

    mpitrace_sortx(profile_elapsed_time, profile_block, profile_sorted_index, SORT_DESCENDING_ORDER);

    fprintf(fh, "\n");
    fprintf(fh,"-----------------------------------------------------------------\n");
    fprintf(fh, "Profile by call site, traceback level = %d\n", traceback_level);
    fprintf(fh,"-----------------------------------------------------------------\n");
    fprintf(fh, "Use addr2line to map the address to source file and line number.\n");
    fprintf(fh, "Ensure -g is used for compilation and linking.                \n");
    fprintf(fh,"-----------------------------------------------------------------\n");
    for (i=0; i<profile_block; i++)
    {
        k = profile_sorted_index[i];
        if (profile_elapsed_time[i] > 1.0e-3)
        {
           fprintf(fh, " \ncommunication time = %.3f sec, call site address = %#10.8x\n",
                   profile_elapsed_time[i], profile_callsite[k]);
           fprintf(fh, "   MPI Routine                  #calls        time(sec)\n");
           for (id=0; id<MAX_IDS; id++)
           {
               if (profile_call_count[k][id] > 0LL)
               {
                   fprintf(fh, "   %-22s %12lld    %12.3f\n",
                           label[id], profile_call_count[k][id], profile_callsite_time[k][id]);
               }
           }
        }
    }
    fprintf(fh, "\n");

    return;
}


static void write_profile_data(void)
{
   int i, k, rc, id, bin, myrank, print_summary;
   double total_comm, total_count, avg_bytes, avg_size, current_time;
   unsigned int min_size, max_size;
   struct timeval TV;
   struct rusage RU;
   int * sorted_rank;
   int task, mintask, mycpu, maxcpu, * all_cpus;
   cpu_set_t cpuset;
   long * all_context_switches;
   long total_bytes_sent;
   double * all_total_comm, * sorted_total_comm, * all_memsizes;
   double * all_elapsed_time, * all_user_time, * all_system_time;
   int min_comm_task, max_comm_task, med_comm_task;
   double min_total_comm, max_total_comm, med_total_comm;
   double mpi_io_time, min_mpi_io_time, max_mpi_io_time, avg_mpi_io_time;
   int mpi_io_routines_called, total_mpi_io_routines_called;
   int my_bin, num_bins = 15;
   int my_histo[15], histo_sum[15];
   double xhisto, histo_bin_width, histo_lower_bound;
   FILE * fh, * fp;
   int nbytes, partners, maxpartners;
   int green_light = 1;
   MPI_Status status;
   struct connStruct {
                        float bytes;
                        int   count;
                        int   rank;
                        int   pad;
                     };
   struct connStruct * connection;
   FILE * fd;
   int max_event_number, * event_number_all_tasks;
   int tag = 100;
   int buffer_overflow_count = 0;
   char * ptr;
   char filename[240], pattern_file[240], trace_file[240];
   struct maxStruct {
                       float data;
                       int rank;
                    };
   struct maxStruct my_mem, max_mem, my_elapsed, max_elapsed;
   char sformat[] = "%-28s %12lld    %11.1f   %12.3f\n";
   char pformat[] = "   %-28s %12lld    %11.1f   %12.3f\n";
   char dformat[] = "                    %12lld   %11.1f   %12.3f\n";

   /*--------------------------------------------------------*/
   /* stop the timers if summary collection is still enabled */
   /*--------------------------------------------------------*/

   if (collect_summary)
   {
      gettimeofday(&TV, NULL);

      current_time = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

      elapsed_time = current_time - elapsed_time_initial;

      exclusive_time += (current_time - exclusive_time_start);

      getrusage(RUSAGE_SELF, &RU);

      user_time = (double) (RU.ru_utime).tv_sec
                + 1.0e-6*((double) (RU.ru_utime).tv_usec) 
                - user_time_initial;

      system_time = (double) (RU.ru_stime).tv_sec
                  + 1.0e-6*((double) (RU.ru_stime).tv_usec) 
                  - system_time_initial;

      max_memory = ((double) RU.ru_maxrss)/1024.0;

//    context_switches = RU.ru_nvcsw + RU.ru_nivcsw - context_switches_initial;
      context_switches = RU.ru_nivcsw - context_switches_initial;

   }

#ifdef HPM
   if (!time_window_profiling) HPM_Stop("mpiAll");
#endif
#ifdef HPMPROF
   if (!time_window_profiling) HPM_Prof_stop();
#endif

   /*----------------------------------------------------*/
   /* compute the total communication time for all tasks */
   /*----------------------------------------------------*/
   total_comm = 0.0;
   for (id=0; id<MIN_MPI_IO_ID; id++)
   {
     if (event_count[id] > 0LL)
     {
       total_comm += total_time[id];
     }
   }

   if (collective_barrier) total_comm += synctime;

   /*-------------------------------------------*/
   /* compute the total time in MPI-IO routines */
   /*-------------------------------------------*/
   mpi_io_time = 0.0;
   mpi_io_routines_called = 0;
   for (id=MIN_MPI_IO_ID; id<MAX_IDS; id++)
   {
     if (event_count[id] > 0LL)
     {
       mpi_io_time += total_time[id];
       mpi_io_routines_called ++;
     }
   }

   PMPI_Allreduce(&mpi_io_routines_called, &total_mpi_io_routines_called, 1,
                 MPI_INT, MPI_SUM, MPI_COMM_WORLD);

   PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);

   if (total_mpi_io_routines_called > 0)
   {
      PMPI_Allreduce(&mpi_io_time, &min_mpi_io_time, 1, MPI_DOUBLE, MPI_MIN, MPI_COMM_WORLD);
      PMPI_Allreduce(&mpi_io_time, &max_mpi_io_time, 1, MPI_DOUBLE, MPI_MAX, MPI_COMM_WORLD);
      PMPI_Allreduce(&mpi_io_time, &avg_mpi_io_time, 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
      avg_mpi_io_time = avg_mpi_io_time / ((double) ntasks);
   }

   sorted_rank = (int *) malloc(ntasks*sizeof(int));
   sorted_total_comm = (double *) malloc(ntasks*sizeof(double));
   all_total_comm = (double *) malloc(ntasks*sizeof(double));
   all_user_time = (double *) malloc(ntasks*sizeof(double));
   all_system_time = (double *) malloc(ntasks*sizeof(double));
   all_elapsed_time = (double *) malloc(ntasks*sizeof(double));
   all_memsizes = (double *) malloc(ntasks*sizeof(double));
   all_context_switches = (long *) malloc(ntasks*sizeof(long));
   all_cpus = (int *) malloc(ntasks*sizeof(int));

   mycpu = sched_getcpu();

   if (all_total_comm != NULL) 
   {
      print_summary = 1;

      PMPI_Allgather(&total_comm, 1, MPI_DOUBLE, all_total_comm, 1, MPI_DOUBLE, MPI_COMM_WORLD);

      PMPI_Gather(&mycpu, 1, MPI_INT, all_cpus, 1, MPI_INT, 0, MPI_COMM_WORLD);

      PMPI_Gather(&elapsed_time, 1, MPI_DOUBLE, all_elapsed_time, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      PMPI_Gather(&user_time, 1, MPI_DOUBLE, all_user_time, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      PMPI_Gather(&system_time, 1, MPI_DOUBLE, all_system_time, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      PMPI_Gather(&context_switches, 1, MPI_LONG, all_context_switches, 1, MPI_LONG, 0, MPI_COMM_WORLD);

      PMPI_Gather(&max_memory, 1, MPI_DOUBLE,
                  all_memsizes, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

      for (i=0; i<ntasks; i++) sorted_total_comm[i] = all_total_comm[i];

      mpitrace_sortx(sorted_total_comm, ntasks, sorted_rank, SORT_ASCENDING_ORDER);

      min_comm_task = sorted_rank[0];
      max_comm_task = sorted_rank[ntasks-1];
      med_comm_task = sorted_rank[(ntasks-1)/2];

      min_total_comm = sorted_total_comm[0];
      max_total_comm = sorted_total_comm[ntasks-1];
      med_total_comm = sorted_total_comm[(ntasks-1)/2];
   }
   else print_summary = 0;

   /*------------------------------------------------------------------------------------*/
   /* find the rank that used the most memory and the one that had the most elapsed time */
   /*------------------------------------------------------------------------------------*/
   my_mem.data = (float) max_memory;
   my_mem.rank = myrank;
   PMPI_Allreduce(&my_mem, &max_mem, 1, MPI_FLOAT_INT, MPI_MAXLOC, MPI_COMM_WORLD);

   my_elapsed.data = (float) elapsed_time;
   my_elapsed.rank = myrank;
   PMPI_Allreduce(&my_elapsed, &max_elapsed, 1, MPI_FLOAT_INT, MPI_MAXLOC, MPI_COMM_WORLD);

   /*----------------------------------------*/
   /* make a histogram of times spent in MPI */
   /*----------------------------------------*/
   for (i=0; i<num_bins; i++) my_histo[i]  = 0;
   for (i=0; i<num_bins; i++) histo_sum[i] = 0;
   histo_bin_width = (max_total_comm - min_total_comm) / ((double) (num_bins - 1)) ;
   histo_lower_bound = min_total_comm - 0.5*histo_bin_width;
   if (histo_bin_width > 0.0) my_bin = (int) ( (total_comm - histo_lower_bound) / histo_bin_width );
   else                       my_bin = 0;
   if (my_bin >= 0 && my_bin < num_bins) my_histo[my_bin] = 1;
   PMPI_Reduce(my_histo, histo_sum, num_bins, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

   if (save_all_tasks) output_filter = 1;
   else if (!use_output_list)
   {
      if (myrank==0 || myrank==min_comm_task || myrank==max_comm_task || myrank==med_comm_task) output_filter = 1;
      else output_filter = 0;
   }

#ifdef HPM
   HPM_Print(jobid, add_timestamp, timestamp, output_filter);
   PMPI_Barrier(MPI_COMM_WORLD);
#endif
#ifdef HPMPROF
   HPM_Prof_write(jobid, add_timestamp, timestamp, output_filter);
   PMPI_Barrier(MPI_COMM_WORLD);
#endif

   /*--------------------------------------------*/
   /* write a text output file with summary data */
   /*--------------------------------------------*/
   if (output_filter)
   {
#ifdef VPROF
      // these just return if vprof was not used
      mpitrace_vprof_stop();  
      mpitrace_vprof_write(jobid, add_timestamp, timestamp);
#endif

      if (add_timestamp) 
      {
         if (trace_dir == NULL) sprintf(filename,    "mpi_profile.%d.%s.%d", jobid, timestamp, myrank);
         else                   sprintf(filename, "%s/mpi_profile.%d.%s.%d", trace_dir, jobid, timestamp, myrank);
      }
      else
      {
         if (trace_dir == NULL) sprintf(filename,    "mpi_profile.%d.%d", jobid, myrank);
         else                   sprintf(filename, "%s/mpi_profile.%d.%d", trace_dir, jobid, myrank);
      }

      fh = fopen(filename, "w");
      if (fh == NULL)
      {
         printf("task %d failed to open the trace file %s ... using stderr\n", myrank, filename);
         fh = stderr; 
      }

      if (comm_profile) 
         fprintf(fh,"Data for MPI rank %d of %d: (all communicators)\n", myrank, ntasks);
      else 
         fprintf(fh,"Data for MPI rank %d of %d:\n", myrank, ntasks);
      if (collect_summary) fprintf(fh,"Times from MPI_Init() to MPI_Finalize().\n");
      else                 fprintf(fh,"Times from MPI_Pcontrol(1) to MPI_Pcontrol(0).\n");
      fprintf(fh,"-----------------------------------------------------------------------\n");
      fprintf(fh,"MPI Routine                        #calls     avg. bytes      time(sec)\n");
      fprintf(fh,"-----------------------------------------------------------------------\n");
      for (id=0; id<MAX_IDS; id++)
      {
        if (event_count[id] > 0LL)
        {
          avg_bytes = total_bytes[id] / ((double) event_count[id]);
          fprintf(fh,sformat, label[id], event_count[id], avg_bytes, total_time[id]);
        }
      }
      fprintf(fh,"-----------------------------------------------------------------------\n");
      if (myrank == min_comm_task)
         fprintf(fh, "MPI task %d of %d had the minimum communication time.\n", myrank, ntasks);
      if (myrank == max_comm_task)
         fprintf(fh, "MPI task %d of %d had the maximum communication time.\n", myrank, ntasks);
      if (myrank == med_comm_task)
         fprintf(fh, "MPI task %d of %d had the median communication time.\n", myrank, ntasks);
      if (collective_barrier)
      {
         fprintf(fh,"synchronization time     = %.3f seconds.\n", synctime);
         fprintf(fh,"total communication time = %.3f seconds (including synchronization).\n", total_comm);
      }
      else fprintf(fh,"total communication time = %.3f seconds.\n", total_comm);
      fprintf(fh,"total elapsed time       = %.3f seconds.\n", elapsed_time);
      if (summary_start_count > 1) fprintf(fh,"exclusive elapsed time   = %.3f seconds.\n", exclusive_time);
      fprintf(fh,"user cpu time            = %.3f seconds.\n", user_time);
      fprintf(fh,"system time              = %.3f seconds.\n", system_time);
      fprintf(fh,"max resident set size    = %.3f MiB.\n", max_memory);
      if (mpi_io_routines_called > 0)
      {
         fprintf(fh,"\n");
         fprintf(fh,"total MPI-IO time = %.3f seconds.\n", mpi_io_time);
      }
      fprintf(fh,"\n");
      fprintf(fh,"-----------------------------------------------------------------\n");
      if (comm_profile)
         fprintf(fh,"Message size distributions: (all communicators)\n\n");
      else 
         fprintf(fh,"Message size distributions:\n\n");
      for (id=0; id<MAX_IDS; id++)
      {
        if ( event_count[id] > 0LL  &&  total_bytes[id] > 0.0 )
        {
          fprintf(fh,"%-22s    #calls    avg. bytes      time(sec)\n", label[id]);
          for (bin=0; bin<MAX_BINS; bin++)
          {
            if (bin_count[id][bin] > 0LL)
            {
              avg_bytes = bin_bytes[id][bin] / ((double) bin_count[id][bin] );
              fprintf(fh,dformat, bin_count[id][bin], avg_bytes, bin_time[id][bin]);
            }
          }
          fprintf(fh,"\n");
        }
      }


      if (comm_profile)
      {
         fprintf(fh,"   -----------------------------------------------------------------\n");
         fprintf(fh,"   Profile data by communicator size:");
         for (bin=0; bin<MAX_BINS; bin++)
         {
            total_count = 0.0;
            total_comm = 0.0;
            for (id=0; id<MIN_MPI_IO_ID; id++) total_count += (double)  comm_count[id][bin];
            if (total_size[bin] > 0.0)
            {
               if (bin == 0) min_size = 0;
               else          min_size = comm_size[bin-1] + 1;
               max_size = comm_size[bin];
               avg_size = total_size[bin]/total_count; 
               fprintf(fh,"\n");
               fprintf(fh,"   -----------------------------------------------------------------------\n");
               fprintf(fh,"   Data for communicator sizes %u to %u , avg size = %.2lf:\n",  min_size, max_size, avg_size);
               fprintf(fh,"   -----------------------------------------------------------------------\n");
               fprintf(fh,"   MPI Routine                        #calls      avg. bytes      time(sec)\n");
               for (id=0; id<MAX_IDS; id++)
               {
                 if ( comm_count[id][bin] > 0LL )
                 {
                    total_comm += comm_time[id][bin];
                    if (comm_bytes[id][bin] < 0.0) avg_bytes = 0.0;
                    else avg_bytes = comm_bytes[id][bin] / ((double) comm_count[id][bin] );
                    fprintf(fh, pformat, label[id], comm_count[id][bin], avg_bytes, comm_time[id][bin]);
                 }
               }
               fprintf(fh, "   MPI time for this communicator bin = %.3lf sec.\n", total_comm);
            }
         }
         fprintf(fh,"\n");
      }

      if (profile_by_call_site) print_profile_by_call_site(fh);

      if (myrank==0 && print_summary)
      {
         fprintf(fh,"-----------------------------------------------------------------\n");
         fprintf(fh, "\nSummary for all tasks:\n");
         fprintf(fh, "\n");
         fprintf(fh, "  Rank %d reported the largest memory utilization : %.2f MiB\n", max_mem.rank, max_mem.data);
         fprintf(fh, "  Rank %d reported the largest elapsed time : %.2f sec\n", max_elapsed.rank, max_elapsed.data);
         fprintf(fh,"\n");
         fprintf(fh, "  minimum communication time = %.3lf sec for task %d\n",
                 min_total_comm, min_comm_task);
         fprintf(fh, "  median  communication time = %.3lf sec for task %d\n",
                 med_total_comm, med_comm_task);
         fprintf(fh, "  maximum communication time = %.3lf sec for task %d\n",
                 max_total_comm, max_comm_task);
         if (total_mpi_io_routines_called > 0)
         {
            fprintf(fh,"\n");
            fprintf(fh, "  minimum MPI-IO time = %.3f seconds\n", min_mpi_io_time);
            fprintf(fh, "  average MPI-IO time = %.3f seconds\n", avg_mpi_io_time);
            fprintf(fh, "  maximum MPI-IO time = %.3f seconds\n", max_mpi_io_time);
         }
         fprintf(fh, "\n\n");
         if (ntasks > 32)
         {
            fprintf(fh, "Histogram of times spent in MPI\n");
            fprintf(fh, "    time-bin    #ranks\n");
            for (i=0; i<num_bins; i++)
            {
               xhisto = min_total_comm + ((double) i) * histo_bin_width;
               fprintf(fh, "%12.3lf  %8d\n", xhisto, histo_sum[i]);
            }
            fprintf(fh, "\n\n");
         }
//       fprintf(fh, "MPI tasks sorted by communication time:\n");
         fprintf(fh, "MPI timing summary for all ranks:\n");
         fprintf(fh, "taskid           hostname    cpu    comm(s)  elapsed(s)     user(s)   system(s)   size(MiB)    switches\n");
         for (i=0; i<ntasks; i++)
         {
//           k = sorted_rank[i];  // for a list sorted by comm time
             k = i;
             ptr = hostnames + k*sizeof(host);
             fprintf(fh, "%6d %18s %6d %10.2lf  %10.2lf  %10.2lf  %10.2lf  %10.2lf  %10ld\n", k, ptr,
                           all_cpus[k], all_total_comm[k], all_elapsed_time[k], all_user_time[k],
                           all_system_time[k], all_memsizes[k], all_context_switches[k]);
         }
      }
   
      if (fh != stderr) fclose(fh);
   }

   /*-----------------------------------------------*/
   /* optionally output data about the send pattern */
   /*-----------------------------------------------*/
   if (trace_send_pattern)
   {
      total_bytes_sent = 0L;
      for (i=0; i<ntasks; i++) total_bytes_sent += (long) send_bytes[i];

      /*----------------------------------------*/
      /* write pattern files for selected ranks */
      /*----------------------------------------*/
      if (output_filter && total_bytes_sent>0L)
      {
         if (add_timestamp)
         {
            if (trace_dir == NULL) sprintf(pattern_file,    "pattern.%d.%s.%d", jobid, timestamp, myrank);
            else                   sprintf(pattern_file, "%s/pattern.%d.%s.%d", trace_dir, jobid, timestamp, myrank);
         }
         else
         {
            if (trace_dir == NULL) sprintf(pattern_file,    "pattern.%d.%d", jobid, myrank);
            else                   sprintf(pattern_file, "%s/pattern.%d.%d", trace_dir, jobid, myrank);
         }

         fp = fopen(pattern_file, "w");
         if (fp == NULL)
         {
            printf("task %d failed to open the pattern file %s\n", myrank, pattern_file);
            fp = stderr;
         }

         fprintf(fp, "MPI rank %d, host %s\n", myrank, host);
         fprintf(fp, "   dest     bytes      count     host\n");
         for (i=0; i<ntasks; i++)
         {
            if (send_bytes[i] > 0.0)
            {
               fprintf(fp, "%7d %10.3e %9d   %s\n", i, send_bytes[i], send_count[i], &hostnames[i*sizeof(host)]);
            }
         }

         if (fp != stderr) fclose(fp);
      }

      /*-----------------------------------------------*/
      /* save a matrix indicating the pattern of sends */
      /*-----------------------------------------------*/
      partners = 0;
      for (i=0; i<ntasks; i++) if (send_bytes[i] > 0.0) partners++;

      PMPI_Allreduce(&partners, &maxpartners, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);

      connection = (struct connStruct *) malloc(maxpartners*sizeof(struct connStruct));


      k = 0;
      for (i=0; i<ntasks; i++)
      {
         if (send_bytes[i] > 0.0)
         {
            connection[k].bytes = send_bytes[i];
            connection[k].count = send_count[i];
            connection[k].rank  = i;
            k++;
         }
      }

      if (myrank == 0)
      {
         if (add_timestamp)
         {
            if (trace_dir == NULL) sprintf(filename, "send_bytes.sparse.%d.%s", jobid, timestamp);
            else                   sprintf(filename, "%s/send_bytes.sparse.%d.%s", trace_dir, jobid, timestamp);
         }
         else
         {
            if (trace_dir == NULL) sprintf(filename, "send_bytes.sparse.%d", jobid);
            else                   sprintf(filename, "%s/send_bytes.sparse.%d", trace_dir, jobid);
         }

         fd = fopen(filename, "w");

         if (fd != NULL) 
         {
            fwrite(&ntasks, sizeof(int), 1, fd);
            fwrite(&partners, sizeof(int), 1, fd);
            fwrite(connection, sizeof(struct connStruct), partners, fd);
         }

         for (id = 1; id<ntasks; id++)
         {
            PMPI_Send(&green_light, 1, MPI_INT, id, tag, MPI_COMM_WORLD);
            PMPI_Recv(&partners, 1, MPI_INT, id, tag, MPI_COMM_WORLD, &status);
            nbytes = partners*sizeof(struct connStruct);
            PMPI_Recv(connection, nbytes, MPI_BYTE, id, tag, MPI_COMM_WORLD, &status);
            if (fd != NULL) 
            {
               fwrite(&partners, sizeof(int), 1, fd);
               fwrite(connection, sizeof(struct connStruct), partners, fd);
            }
         }

         if (fd != NULL)
         {
           fclose(fd);
           printf("wrote file %s\n", filename);
         }
      }
      else
      {
         PMPI_Recv(&green_light, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
         PMPI_Send(&partners, 1, MPI_INT, 0, tag, MPI_COMM_WORLD);
         nbytes = partners*sizeof(struct connStruct);
         PMPI_Send(connection, nbytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
      }
   }


   /*---------------------------------------------------*/
   /* save a binary trace file with data from all tasks */
   /*---------------------------------------------------*/
   PMPI_Allreduce(&event_number, &max_event_number, 1, MPI_INT, MPI_MAX, MPI_COMM_WORLD);
   if (max_event_number > 0)
   {
     event_number_all_tasks = (int *) malloc(ntasks*sizeof(int));

     PMPI_Reduce(&event_buffer_overflow, &buffer_overflow_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    
     if (add_timestamp)
     {
        if (trace_dir == NULL) sprintf(trace_file,    "%d.%s.trc", jobid, timestamp);
        else                   sprintf(trace_file, "%s/%d.%s.trc", trace_dir, jobid, timestamp);
     }
     else
     {
        if (trace_dir == NULL) sprintf(trace_file,    "%d.trc", jobid);
        else                   sprintf(trace_file, "%s/%d.trc", trace_dir, jobid);
     }

     if (myrank == 0) fd = fopen(trace_file, "wb");

     PMPI_Gather(&event_number,1, MPI_INT, 
                 event_number_all_tasks, 1, MPI_INT, 0, MPI_COMM_WORLD);

     if (trace_min_rank == 0) mintask = 1;
     else                     mintask = trace_min_rank;
 
     if (myrank == 0) 
     {
        nbytes = event_number * sizeof(struct eventstruct);

        if ((fd >= 0) && (trace_min_rank == 0)) write_tracefile(fd, event, nbytes);

        for (task = mintask; task <= trace_max_rank; task++)
        {
           PMPI_Send(&green_light, 1, MPI_INT, task, tag, MPI_COMM_WORLD);
           nbytes = event_number_all_tasks[task] * sizeof(struct eventstruct);
           PMPI_Recv(event, nbytes, MPI_BYTE, task, tag, MPI_COMM_WORLD, &status);
           if (fd >= 0) write_tracefile(fd, event, nbytes);
        }

        fclose(fd);

        printf("wrote trace file: %s\n", trace_file);

        if (buffer_overflow_count > 0) 
           printf("event buffer overflow occured on %d tasks.\n", buffer_overflow_count);
     }
     else if ((myrank >= trace_min_rank) && (myrank <= trace_max_rank))
     {
        PMPI_Recv(&green_light, 1, MPI_INT, 0, tag, MPI_COMM_WORLD, &status);
        nbytes = event_number * sizeof(struct eventstruct);
        PMPI_Send(event, nbytes, MPI_BYTE, 0, tag, MPI_COMM_WORLD);
     }
   }

  
   /*--------------------------------------------------*/
   /* for systems that don't label gmon.out files make */
   /* a directory to save output from specific ranks   */
   /*--------------------------------------------------*/
   if (output_filter && pg_profile) 
   {
      if (add_timestamp)
      {
         if (trace_dir == NULL) sprintf(filename, "pgdir.%d.%s", taskid, timestamp);
         else                   sprintf(filename, "%s/pgdir.%d.%s", trace_dir, taskid, timestamp);
      }
      else
      {
         if (trace_dir == NULL) sprintf(filename, "pgdir.%d", taskid);
         else                   sprintf(filename, "%s/pgdir.%d", trace_dir, taskid);
      }
      rc = mkdir(filename, 0755);
      if (rc == 0) rc = chdir(filename);
   }

   return;
}

static void write_profile_data_myrank(void)
{
   int i, k, rc, id, bin, myrank;
   double total_comm, total_count, avg_bytes, avg_size, current_time;
   unsigned int min_size, max_size;
   struct timeval TV;
   struct rusage RU;
   long total_bytes_sent;
   int min_comm_task, max_comm_task, med_comm_task;
   double min_total_comm, max_total_comm, med_total_comm;
   double mpi_io_time, min_mpi_io_time, max_mpi_io_time, avg_mpi_io_time;
   int mpi_io_routines_called, total_mpi_io_routines_called;
   FILE * fh, * fp;
   int nbytes, partners, maxpartners;
   MPI_Status status;
   FILE * fd;
   int tag = 100;
   int buffer_overflow_count = 0;
   char * ptr;
   char filename[240];
   char pattern_file[240];
   struct maxStruct {
                       float data;
                       int rank;
                    };
   struct maxStruct my_mem, max_mem, my_elapsed, max_elapsed;
   char sformat[] = "%-28s %12lld    %11.1f   %12.3f\n";
   char pformat[] = "   %-28s %12lld    %11.1f   %12.3f\n";
   char dformat[] = "                    %12lld   %11.1f   %12.3f\n";

   /*--------------------------------------------------------*/
   /* stop the timers if summary collection is still enabled */
   /*--------------------------------------------------------*/

   PMPI_Comm_rank(MPI_COMM_WORLD, &myrank);

   if (collect_summary)
   {
      gettimeofday(&TV, NULL);

      current_time = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);

      elapsed_time = current_time - elapsed_time_initial;

      exclusive_time += (current_time - exclusive_time_start);

      getrusage(RUSAGE_SELF, &RU);

      user_time = (double) (RU.ru_utime).tv_sec
                + 1.0e-6*((double) (RU.ru_utime).tv_usec) 
                - user_time_initial;

      system_time = (double) (RU.ru_stime).tv_sec
                  + 1.0e-6*((double) (RU.ru_stime).tv_usec) 
                  - system_time_initial;

      max_memory = ((double) RU.ru_maxrss)/1024.0;

//    context_switches = RU.ru_nvcsw + RU.ru_nivcsw - context_switches_initial;
      context_switches = RU.ru_nivcsw - context_switches_initial;

   }

#ifdef HPM
   if (!time_window_profiling) HPM_Stop("mpiAll");
#endif
#ifdef HPMPROF
   HPM_Prof_stop();
#endif

   /*----------------------------------------------------*/
   /* compute the total communication time for all tasks */
   /*----------------------------------------------------*/
   total_comm = 0.0;
   for (id=0; id<MIN_MPI_IO_ID; id++)
   {
     if (event_count[id] > 0LL)
     {
       total_comm += total_time[id];
     }
   }

   if (collective_barrier) total_comm += synctime;

   /*-------------------------------------------*/
   /* compute the total time in MPI-IO routines */
   /*-------------------------------------------*/
   mpi_io_time = 0.0;
   mpi_io_routines_called = 0;
   for (id=MIN_MPI_IO_ID; id<MAX_IDS; id++)
   {
     if (event_count[id] > 0LL)
     {
       mpi_io_time += total_time[id];
       mpi_io_routines_called ++;
     }
   }

   if (save_all_tasks) output_filter = 1;

#ifdef HPM
   HPM_Print_myrank(jobid, add_timestamp, timestamp, output_filter);
#endif
#ifdef HPMPROF
   HPM_Prof_write(jobid, add_timestamp, timestamp, output_filter);
#endif

   /*--------------------------------------------*/
   /* write a text output file with summary data */
   /*--------------------------------------------*/
   if (output_filter)
   {
#ifdef VPROF
      // these just return if vprof was not used
      mpitrace_vprof_stop();  
      mpitrace_vprof_write(jobid, add_timestamp, timestamp);
#endif

      if (add_timestamp) 
      {    
         if (trace_dir == NULL) sprintf(filename,    "mpi_profile.%d.%s.%d", jobid, timestamp, myrank);
         else                   sprintf(filename, "%s/mpi_profile.%d.%s.%d", trace_dir, jobid, timestamp, myrank);
      }    
      else 
      {    
         if (trace_dir == NULL) sprintf(filename,    "mpi_profile.%d.%d", jobid, myrank);
         else                   sprintf(filename, "%s/mpi_profile.%d.%d", trace_dir, jobid, myrank);
      }

      fh = fopen(filename, "w");
      if (fh == NULL)
      {
         printf("task %d failed to open the trace file %s ... using stderr\n", myrank, filename);
         fh = stderr; 
      }

      if (comm_profile) 
         fprintf(fh,"Data for MPI rank %d of %d: (all communicators)\n", myrank, ntasks);
      else 
         fprintf(fh,"Data for MPI rank %d of %d:\n", myrank, ntasks);
      if (collect_summary) fprintf(fh,"Times from MPI_Init() to MPI_Finalize().\n");
      else                 fprintf(fh,"Times from MPI_Pcontrol(1) to MPI_Pcontrol(0).\n");
      fprintf(fh,"-----------------------------------------------------------------------\n");
      fprintf(fh,"MPI Routine                        #calls     avg. bytes      time(sec)\n");
      fprintf(fh,"-----------------------------------------------------------------------\n");
      for (id=0; id<MAX_IDS; id++)
      {
        if (event_count[id] > 0LL)
        {
          avg_bytes = total_bytes[id] / ((double) event_count[id]);
          fprintf(fh,sformat, label[id], event_count[id], avg_bytes, total_time[id]);
        }
      }
      fprintf(fh,"-----------------------------------------------------------------------\n");
      if (collective_barrier)
      {
         fprintf(fh,"synchronization time     = %.3f seconds.\n", synctime);
         fprintf(fh,"total communication time = %.3f seconds (including synchronization).\n", total_comm);
      }
      else fprintf(fh,"total communication time = %.3f seconds.\n", total_comm);
      fprintf(fh,"total elapsed time       = %.3f seconds.\n", elapsed_time);
      if (summary_start_count > 1) fprintf(fh,"exclusive elapsed time   = %.3f seconds.\n", exclusive_time);
      fprintf(fh,"user cpu time            = %.3f seconds.\n", user_time);
      fprintf(fh,"system time              = %.3f seconds.\n", system_time);
      fprintf(fh,"max resident set size    = %.3f MiB.\n", max_memory);
      if (mpi_io_routines_called > 0)
      {
         fprintf(fh,"\n");
         fprintf(fh,"total MPI-IO time = %.3f seconds.\n", mpi_io_time);
      }
      fprintf(fh,"\n");
      fprintf(fh,"-----------------------------------------------------------------\n");
      if (comm_profile)
         fprintf(fh,"Message size distributions: (all communicators)\n\n");
      else 
         fprintf(fh,"Message size distributions:\n\n");
      for (id=0; id<MAX_IDS; id++)
      {
        if ( event_count[id] > 0LL  &&  total_bytes[id] > 0.0 )
        {
          fprintf(fh,"%-22s    #calls    avg. bytes      time(sec)\n", label[id]);
          for (bin=0; bin<MAX_BINS; bin++)
          {
            if (bin_count[id][bin] > 0LL)
            {
              avg_bytes = bin_bytes[id][bin] / ((double) bin_count[id][bin] );
              fprintf(fh,dformat, bin_count[id][bin], avg_bytes, bin_time[id][bin]);
            }
          }
          fprintf(fh,"\n");
        }
      }


      if (comm_profile)
      {
         fprintf(fh,"   -----------------------------------------------------------------\n");
         fprintf(fh,"   Profile data by communicator size:");
         for (bin=0; bin<MAX_BINS; bin++)
         {
            total_count = 0.0;
            total_comm = 0.0;
            for (id=0; id<MIN_MPI_IO_ID; id++) total_count += (double)  comm_count[id][bin];
            if (total_size[bin] > 0.0)
            {
               if (bin == 0) min_size = 0;
               else          min_size = comm_size[bin-1] + 1;
               max_size = comm_size[bin];
               avg_size = total_size[bin]/total_count; 
               fprintf(fh,"\n");
               fprintf(fh,"   -----------------------------------------------------------------------\n");
               fprintf(fh,"   Data for communicator sizes %u to %u , avg size = %.2lf:\n",  min_size, max_size, avg_size);
               fprintf(fh,"   -----------------------------------------------------------------------\n");
               fprintf(fh,"   MPI Routine                        #calls      avg. bytes      time(sec)\n");
               for (id=0; id<MAX_IDS; id++)
               {
                 if ( comm_count[id][bin] > 0LL )
                 {
                    total_comm += comm_time[id][bin];
                    if (comm_bytes[id][bin] < 0.0) avg_bytes = 0.0;
                    else avg_bytes = comm_bytes[id][bin] / ((double) comm_count[id][bin] );
                    fprintf(fh, pformat, label[id], comm_count[id][bin], avg_bytes, comm_time[id][bin]);
                 }
               }
               fprintf(fh, "   MPI time for this communicator bin = %.3lf sec.\n", total_comm);
            }
         }
         fprintf(fh,"\n");
      }

      if (profile_by_call_site) print_profile_by_call_site(fh);

      if (fh != stderr) fclose(fh);

   }

   /*-----------------------------------------------*/
   /* optionally output data about the send pattern */
   /*-----------------------------------------------*/
   if (trace_send_pattern)
   {
      total_bytes_sent = 0L;
      for (i=0; i<ntasks; i++) total_bytes_sent += (long) send_bytes[i];

      /*----------------------------------------*/
      /* write pattern files for selected ranks */
      /*----------------------------------------*/
      if (output_filter && total_bytes_sent>0L)
      {
         if (trace_dir == NULL) sprintf(pattern_file,    "pattern.%d.%d", jobid, myrank);
         else                   sprintf(pattern_file, "%s/pattern.%d.%d", trace_dir, jobid, myrank);

         fp = fopen(pattern_file, "w");
         if (fp == NULL)
         {
            printf("task %d failed to open the pattern file %s\n", myrank, pattern_file);
            fp = stderr;
         }

         fprintf(fp, "MPI rank %d, host %s\n", myrank, host);
         fprintf(fp, "   dest     bytes      count     host\n");
         for (i=0; i<ntasks; i++)
         {
            if (send_bytes[i] > 0.0)
            {
               fprintf(fp, "%7d %10.3e %9d   %s\n", i, send_bytes[i], send_count[i], &hostnames[i*sizeof(host)]);
            }
         }

         if (fp != stderr) fclose(fp);
      }
   }

   return;
}
