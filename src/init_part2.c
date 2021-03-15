/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

   rc = PMPI_Comm_rank(MPI_COMM_WORLD, &taskid);

   rc = PMPI_Comm_size(MPI_COMM_WORLD, &ntasks);

   rc = PMPI_Comm_group(MPI_COMM_WORLD, &world_group);

   // check for SLURM, LSF, or PBS, use jobid = pid if not found
   ptr = getenv("SLURM_JOBID");
   if (ptr == NULL) ptr = getenv("PBS_JOBID");
   if (ptr == NULL) ptr = getenv("LSB_JOBID");
   if (ptr == NULL) ptr = getenv("COBALT_JOBID");
   if (ptr != NULL) jobid = atoi(ptr);
   else             jobid = getpid();
   PMPI_Bcast(&jobid, 1, MPI_INT, 0, MPI_COMM_WORLD);

   // optionally add a timestamp to the output files
   ptr = getenv("ADD_TIMESTAMP");
   if (ptr == NULL) add_timestamp = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      add_timestamp = 1;
      if (taskid == 0) 
      {
         current_time = time(NULL);
         timeVal = * localtime(&current_time);
         sprintf(timestamp, "%02d%02d%02d%02d%02d", 1 + timeVal.tm_mon, timeVal.tm_mday, timeVal.tm_hour, timeVal.tm_min, timeVal.tm_sec);
      }
      PMPI_Bcast(timestamp, sizeof(timestamp), MPI_CHAR, 0, MPI_COMM_WORLD);
   }

   hostnames = (char *) malloc(ntasks*sizeof(host));

   gethostname(host, sizeof(host));

   for (i=0; i<sizeof(host); i++) 
   {    
       if (host[i] == '.') 
       {
           host[i] = '\0';
           break;
       }
   }

   PMPI_Allgather(host, sizeof(host), MPI_BYTE, hostnames, sizeof(host), MPI_BYTE, MPI_COMM_WORLD);

   /*-----------------------------------------*/
   /* check env variables for tracing options */
   /*-----------------------------------------*/

   /* control trace generation */
   ptr = getenv("TRACE_ALL_EVENTS");
   if (ptr == NULL) trace_events = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      trace_events = 1;
   }
   else trace_events = 0;

   ptr = getenv("ENABLE_TRACING");
   if (ptr == NULL) enable_tracing = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      enable_tracing = 1;
   }

   ptr = getenv("TRACE_BUFFER_SIZE");
   if (ptr != NULL)
   {
      max_events = atoi(ptr) / sizeof(struct eventstruct);
   }
   else max_events = 100000;

   ptr = getenv("PROFILE_BEGIN_TIME");
   if (ptr !=NULL)
   {
      profile_begin_time = (double) atof(ptr);
      time_window_profiling = 1;
   }

   ptr = getenv("PROFILE_END_TIME");
   if (ptr !=NULL)
   {
      profile_end_time = (double) atof(ptr);
      if (profile_end_time > profile_begin_time) time_window_profiling = 1;
      else                                       time_window_profiling = 0;
   }
   else time_window_profiling = 0;

   ptr = getenv("TRACE_BEGIN_TIME");
   if (ptr !=NULL)
   {
      trace_begin_time = (double) atof(ptr);
      time_window_tracing = 1;
   }

   ptr = getenv("TRACE_END_TIME");
   if (ptr !=NULL)
   {
      trace_end_time = (double) atof(ptr);
      if (trace_end_time > trace_begin_time) time_window_tracing = 1;
      else                                   time_window_tracing = 0;
   }
   else time_window_tracing = 0;
   
   event = (struct eventstruct *) malloc(max_events*sizeof(struct eventstruct));
   if (event == NULL)
   {
      if (taskid == 0)
          printf("malloc failed for the events array, tracing is disabled\n");
      trace_events = 0;
   }

   trace_dir = getenv("TRACE_DIR");

   /* control summary file format */
   ptr = getenv("SAVE_ALL_TASKS");
   if (ptr == NULL) save_all_tasks = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      save_all_tasks = 1;
   }
   else save_all_tasks = 0;

   if (taskid == 0) output_filter = 1;
   else             output_filter = 0;

   list_ptr = getenv("SAVE_LIST");
   if (list_ptr != NULL)
   {
      memset(envstr, '\0', sizeof(envstr));
      strcpy(envstr, list_ptr);
      use_output_list = 1;
      ptr = strtok(envstr, delimiters);
      while(ptr != NULL)
      {
         if (atoi(ptr) == taskid) 
         {
            output_filter = 1;
            break;
         }
         ptr = strtok(NULL, delimiters);
      }
   }

   /*------------------------------------------------------------------*/
   /* check env variable "TRACE_DISABLE_LIST" for routines to turn off */
   /*------------------------------------------------------------------*/
   for (id=0; id<MAX_IDS; id++) disable_tracing[id] = 0;

   list_ptr = getenv("TRACE_DISABLE_LIST");

   if (list_ptr != NULL)
   {   
      ptr = strtok(list_ptr, delimiters);
      while (ptr != NULL)
      {   
         for (id=0; id<MAX_IDS; id++)
         {
            if (strncasecmp(ptr, label[id], strlen(ptr)) == 0)
            {
               if (taskid == 0) printf("disabling event tracing for %s\n", label[id]);
               disable_tracing[id] = 1;
               break;
            }
         }
         ptr = strtok(NULL, delimiters);
      }   
   }

   /*---------------------------------------*/
   /* limit event tracing to taskid < 256   */
   /* unless TRACE_ALL_TASKS = yes          */
   /* or if  TRACE_MAX_RANK is set          */
   /*---------------------------------------*/
   trace_max_rank = (ntasks < 256) ? ntasks - 1 : 255;
   ptr = getenv("TRACE_ALL_TASKS");
   if (ptr != NULL)
   {
      if (strncasecmp(ptr,"yes",3) == 0) trace_max_rank = ntasks - 1;
   }

   ptr = getenv("TRACE_ALL_RANKS");
   if (ptr != NULL)
   {
      if (strncasecmp(ptr,"yes",3) == 0) trace_max_rank = ntasks - 1;
   }

   ptr = getenv("TRACE_MAX_RANK");
   if (ptr != NULL) trace_max_rank = atoi(ptr);
   if (trace_max_rank >= ntasks) trace_max_rank = ntasks - 1;

   ptr = getenv("TRACE_MIN_RANK");
   if (ptr != NULL) trace_min_rank = atoi(ptr);
   if (trace_min_rank > trace_max_rank) trace_min_rank = trace_max_rank;

   ptr = getenv("PROFILE_BY_COMMUNICATOR");
   if (ptr == NULL) comm_profile = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
        comm_profile = 1;
   }
   else comm_profile = 0;

   if (comm_profile) 
   {
      for (id=0; id<MAX_IDS; id++)
      {
         for (bin=0; bin<MAX_BINS; bin++)
         {
            comm_count[id][bin] = 0L;
            comm_bytes[id][bin] = 0.0;
            comm_time[id][bin]  = 0.0;
         }
      }

      for (bin=0; bin<MAX_BINS; bin++) total_size[bin] = 0.0;

      limit = 4;
      comm_size[0] = limit;
      for (bin=1; bin<MAX_BINS; bin++)
      {
         limit *= 2 ;
         comm_size[bin] = limit;
      }
      
   }

#ifdef USE_NVTX   
   if (time_window_profiling)  {
   /* disable cuda profiling from MPI_Init when using time window profiling */
      cudaProfilerStop();
   }   
   else {
   /* use the TRACE* controls to start/stop cuda/nvtx profiling */
      if ((trace_events==1) && (taskid>=trace_min_rank) && (taskid <=trace_max_rank)) cudaProfilerStart();
      else                                                                            cudaProfilerStop();
   }
#endif

   ptr = getenv("VPROF_PROFILE");
   if (ptr == NULL) vprof_profile = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
        vprof_profile = 1;
   }
   else vprof_profile = 0;

   ptr = getenv("PG_PROFILE");
   if (ptr == NULL) pg_profile = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
        pg_profile = 1;
   }
   else pg_profile = 0;

   for (id=0; id<MAX_IDS; id++) barrier_flag[id] = 0;
   list_ptr = getenv("COLLECTIVE_BARRIER");
   if (list_ptr == NULL) collective_barrier = 0;
   else  
   {
      collective_barrier = 1;
      if (strncasecmp(list_ptr,"yes",3) == 0 || strncasecmp(list_ptr,"all",3) == 0)
      {
         for (id=0; id<MAX_IDS; id++) barrier_flag[id] = 1;
      }
      else
      {
         ptr = strtok(list_ptr, delimiters);
         while(ptr != NULL)
         {
            for (id=0; id<MAX_IDS; id++)
            {
               if (strncasecmp(ptr,label[id],strlen(label[id])) == 0) barrier_flag[id] = 1;
            }
            ptr = strtok(NULL, delimiters);
         }
      }
   }

   ptr = getenv("PROFILE_BY_CALL_STACK");
   if (ptr == NULL) profile_by_call_stack = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
        profile_by_call_stack = 1;
   }
   else profile_by_call_stack = 0;

   ptr = getenv("MAX_PROFILE_BLOCKS");
   if (ptr == NULL)                        max_profile_blocks = 10000;
   else if (strncasecmp(ptr,"yes",3) == 0) max_profile_blocks = atoi(ptr);

   ptr = getenv("MAX_STACK_DEPTH");
   if (ptr == NULL)                        max_stack_depth = 4;
   else if (strncasecmp(ptr,"yes",3) == 0) max_stack_depth = atoi(ptr);

   ptr = getenv("MAX_REPORTED_STACKS");
   if (ptr == NULL)                        max_reported_stacks = 50;
   else if (strncasecmp(ptr,"yes",3) == 0) max_reported_stacks = atoi(ptr);

   if (profile_by_call_stack)
   {
      profile_elapsed_time = (double *) malloc(max_profile_blocks*sizeof(double));

      profile_key = (int *) malloc(max_profile_blocks*sizeof(int));

      profile_stack = (long **) malloc(max_profile_blocks*sizeof(long *));
      profile_stack[0] = (long *) malloc(max_profile_blocks*max_stack_depth*sizeof(long));
      for (i=1; i<max_profile_blocks; i++) profile_stack[i] = profile_stack[0] + i*max_stack_depth;

      profile_call_count = (long **) malloc(max_profile_blocks*sizeof(long *));
      profile_call_count[0] = (long *) malloc(max_profile_blocks*MAX_IDS*sizeof(long));
      for (i=1; i<max_profile_blocks; i++) profile_call_count[i] = profile_call_count[0] + i*MAX_IDS;

      profile_function_time = (double **) malloc(max_profile_blocks*sizeof(double *));
      profile_function_time[0] = (double *) malloc(max_profile_blocks*MAX_IDS*sizeof(double));
      for (i=1; i<max_profile_blocks; i++) profile_function_time[i] = profile_function_time[0] + i*MAX_IDS;

      for (i=0; i<max_profile_blocks; i++)
      {
          profile_elapsed_time[i] = 0.0;

          for (j=0; j<max_stack_depth; j++) profile_stack[i][j] = 0L;

          for (id=0; id<MAX_IDS; id++)
          {
              profile_call_count[i][id] = 0L;
              profile_function_time[i][id] = 0.0;
          }
      }
   }

   /* option to set a traceback level */
   ptr = getenv("TRACEBACK_LEVEL");
   if (ptr == NULL) traceback_level = 0;
   else             traceback_level = atoi(ptr) - 1;

   /* option to install an error handler with traceback */
   ptr = getenv("TRACEBACK_ERRORS");
   if (ptr != NULL)
   {
      if (strncasecmp(ptr,"yes",3) == 0)
      {
         MPI_Comm_create_errhandler((MPI_Comm_errhandler_function *) mpitrace_handler, &err_traceback);
         MPI_Comm_set_errhandler(MPI_COMM_WORLD, err_traceback);
      }
   }

   traceback_abort = 0;
   ptr = getenv("TRACEBACK_ABORT");
   if (ptr != NULL)
   {
      if (strncasecmp(ptr,"yes",3) == 0) traceback_abort = 1;
   }

   ptr = getenv("TRACE_SEND_PATTERN");
   if (ptr == NULL) trace_send_pattern = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      trace_send_pattern = 1;

      send_bytes = (float *) malloc(ntasks*sizeof(float));
      send_count = (int *)   malloc(ntasks*sizeof(int));

      for (i=0; i<ntasks; i++) send_bytes[i] = 0.0f;
      for (i=0; i<ntasks; i++) send_count[i] = 0;
   }
   else trace_send_pattern = 0;

   ptr = getenv("TRANSLATE_RANKS");
   if (ptr == NULL) translate_ranks = 1;
   else if (strncasecmp(ptr,"no",2) == 0) translate_ranks  = 0;
   else translate_ranks = 1;

   /* option to swap byte-order in the binary trace file */
   ptr = getenv("SWAP_BYTES");
   if (ptr == NULL) swap_bytes = 0;
   else if (strncasecmp(ptr,"yes",3) == 0)
   {
      swap_bytes = 1;
   }
   else swap_bytes = 0;

   rc = PMPI_Barrier(MPI_COMM_WORLD);

   gettimeofday(&TV, NULL);

   mpi_init_time = (double) TV.tv_sec + 1.0e-6 * ((double) TV.tv_usec);
   elapsed_time_initial = mpi_init_time;

#ifdef HPM
   HPM_Init();
   if (!time_window_profiling) HPM_Start("mpiAll");
#endif
#ifdef HPMPROF
   HPM_Prof_init();
   HPM_Prof_Auto_start();
#endif

#ifdef VPROF
   if (vprof_profile) 
   {
      /* when using an output list enable interrupts selectively */
      if (use_output_list) 
      {
         if (output_filter) mpitrace_vprof_start();
      }
      else mpitrace_vprof_start();
   }
#endif

   getrusage(RUSAGE_SELF, &RU);

   user_time_initial = (double) (RU.ru_utime).tv_sec
             + 1.0e-6*((double) (RU.ru_utime).tv_usec);

   system_time_initial = (double) (RU.ru_stime).tv_sec
               + 1.0e-6*((double) (RU.ru_stime).tv_usec);

// context_switches_initial = RU.ru_nvcsw + RU.ru_nivcsw;
   context_switches_initial = RU.ru_nivcsw;

