/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

/*==========================================================*/
/* Profiling Wrappers for MPI functions, Fortran and C.     */
/* Timing data is reported when MPI_Finalize is called.     */
/* Please e-mail corrections/errors to walkup@us.ibm.com.   */
/*==========================================================*/
#include "mpitrace.h"
#include "mpitrace_ids.h"


#include "mpitrace_common.c"

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Init                               */
/*----------------------------------------------------------*/
int MPI_Init(int * argc, char *** argv)
{
   int i, id, rc, rcinit, bin, namelen;
   unsigned int limit;
   struct timeval TV;
   struct rusage RU;
   struct tm timeVal;
   time_t current_time;
   double frequency;
   char * ptr;
   char * list_ptr;
   char delimiters[] = {","};
   char envstr[320];
   MPI_Errhandler err_traceback;

#include "init_part1.c"

   rcinit = PMPI_Init(argc, argv);

#include "init_part2.c"

   return rcinit;
}


/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Init_thread                        */
/*----------------------------------------------------------*/
#ifdef HAS_INIT_THREAD
int MPI_Init_thread(int * argc, char *** argv, int required, int * provided)
{
   int i, id, rc, rcinit, bin, namelen;
   unsigned int limit;
   struct timeval TV;
   struct rusage RU;
   struct tm timeVal;
   time_t current_time;
   double frequency;
   char * ptr;
   char * list_ptr;
   char delimiters[] = {","};
   char envstr[320];
   MPI_Errhandler err_traceback;

#include "init_part1.c"

   rcinit = PMPI_Init_thread(argc, argv, required, provided);

#include "init_part2.c"

   return rcinit;
}
#endif


/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Pcontrol                           */
/*----------------------------------------------------------*/
int MPI_Pcontrol(const int level, ...)
{
    if (level == 1)   mpitrace_summary_start();
    if (level == 0)   mpitrace_summary_stop();
    if (level == 101) mpitrace_trace_start();
    if (level == 100) mpitrace_trace_stop();
    if (enable_tracing && (level == 1)) mpitrace_trace_start();
    if (enable_tracing && (level == 0)) mpitrace_trace_stop();
#ifdef VPROF
    if (level == 11)  mpitrace_vprof_start();
    if (level == 10)  mpitrace_vprof_stop();
#endif
#ifdef HPMPROF
    if (level == 1)   HPM_Prof_start();
    if (level == 0)   HPM_Prof_stop();
    if (level == 21)  HPM_Prof_start();
    if (level == 20)  HPM_Prof_stop();
#endif
#ifdef HPM
    if (level == 1)  HPM_Start("pcontrol");
    if (level == 0)  HPM_Stop("pcontrol");
#endif

    return 0;
}


/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Comm_rank                          */
/*----------------------------------------------------------*/
int MPI_Comm_rank(MPI_Comm comm, int * id)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[COMM_RANK_ID]);
  
   WTIME(TV1);
   rc = PMPI_Comm_rank(comm, id);
   WTIME(TV2);
  
   nvtxRangeEnd(range);
   LogEvent(COMM_RANK_ID, TV1, TV2, -1, -1, -1, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Comm_size                          */
/*----------------------------------------------------------*/
int MPI_Comm_size(MPI_Comm comm, int * ptasks)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[COMM_SIZE_ID]);

   WTIME(TV1);
   rc = PMPI_Comm_size(comm, ptasks);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(COMM_SIZE_ID, TV1, TV2, -1, -1, -1, comm);
   return rc;
}


/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Send                               */
/*----------------------------------------------------------*/
int MPI_Send(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
             int tag, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SEND_ID]);

   WTIME(TV1);
   rc = PMPI_Send(sbuf, count, type, dest, tag, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(SEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ssend                              */
/*----------------------------------------------------------*/
int MPI_Ssend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
              int tag, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Ssend(sbuf, count, type, dest, tag, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(SSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Rsend                              */
/*----------------------------------------------------------*/
int MPI_Rsend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
              int tag, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Rsend(sbuf, count, type, dest, tag, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(RSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Bsend                              */
/*----------------------------------------------------------*/
int MPI_Bsend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
              int tag, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Bsend(sbuf, count, type, dest, tag, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(BSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Isend                              */
/*----------------------------------------------------------*/
int MPI_Isend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
              int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISEND_ID]);

   WTIME(TV1);
   rc = PMPI_Isend(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(ISEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Issend                             */
/*----------------------------------------------------------*/
int MPI_Issend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
               int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Issend(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(ISSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Irsend                             */
/*----------------------------------------------------------*/
int MPI_Irsend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
               int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IRSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Irsend(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(IRSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ibsend                             */
/*----------------------------------------------------------*/
int MPI_Ibsend(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
               int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBSEND_ID]);

   WTIME(TV1);
   rc = PMPI_Ibsend(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(IBSEND_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Send_init                          */
/*----------------------------------------------------------*/
int MPI_Send_init(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
                  int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SEND_INIT_ID]);

   WTIME(TV1);
   rc = PMPI_Send_init(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(SEND_INIT_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ssend_init                         */
/*----------------------------------------------------------*/
int MPI_Ssend_init(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
                   int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SSEND_INIT_ID]);

   WTIME(TV1);
   rc = PMPI_Ssend_init(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(SSEND_INIT_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Rsend_init                         */
/*----------------------------------------------------------*/
int MPI_Rsend_init(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
                   int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RSEND_INIT_ID]);

   WTIME(TV1);
   rc = PMPI_Rsend_init(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(RSEND_INIT_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Bsend_init                         */
/*----------------------------------------------------------*/
int MPI_Bsend_init(sbuf_t sbuf, int count, MPI_Datatype type, int dest, 
                   int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BSEND_INIT_ID]);

   WTIME(TV1);
   rc = PMPI_Bsend_init(sbuf, count, type, dest, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (dest == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(BSEND_INIT_ID, TV1, TV2, -1, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Recv_init                          */
/*----------------------------------------------------------*/
int MPI_Recv_init(void * sbuf, int count, MPI_Datatype type, int src, 
                  int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RECV_INIT_ID]);

   WTIME(TV1);
   rc = PMPI_Recv_init(sbuf, count, type, src, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (src == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(RECV_INIT_ID, TV1, TV2, src, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Recv                               */
/*----------------------------------------------------------*/
int MPI_Recv(void * rbuf, int count, MPI_Datatype type, int src, 
             int tag, MPI_Comm comm, MPI_Status * status)
{
   int rc, bytes, source, count_received, flag;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RECV_ID]);
   MPI_Status local_status;

   if (barrier_flag[RECV_ID])
   {
      flag = 0;
      WTIME(TV1);
      while (!flag)
      {
         WTIME(TV2);
         PMPI_Iprobe(src, tag, comm, &flag, &local_status);
      }
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Recv(rbuf, count, type, src, tag, comm, status);
   WTIME(TV2);

   if (status != MPI_STATUS_IGNORE)
   {
       PMPI_Get_count(status, type, &count_received);
       source = status->MPI_SOURCE;
   }
   else 
   {
       count_received = count;
       source = src;
   }

   PMPI_Type_size(type, &bytes);
   bytes = count_received * bytes;

   if (src == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(RECV_ID, TV1, TV2, source, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Irecv                              */
/*----------------------------------------------------------*/
int MPI_Irecv(void * rbuf, int count, MPI_Datatype type, int src, 
              int tag, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IRECV_ID]);

   WTIME(TV1);
   rc = PMPI_Irecv(rbuf, count, type, src, tag, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   if (src == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(IRECV_ID, TV1, TV2, src, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Sendrecv                           */
/*----------------------------------------------------------*/
int MPI_Sendrecv(sbuf_t sbuf, int scount, MPI_Datatype stype, int dest, int stag,
                 void * rbuf, int rcount, MPI_Datatype rtype, int src, int rtag,
                 MPI_Comm comm, MPI_Status * status)
{
   int rc, sbytes, rbytes, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SENDRECV_ID]);

   WTIME(TV1);
   rc = PMPI_Sendrecv(sbuf, scount, stype, dest, stag,
                      rbuf, rcount, rtype, src, rtag,
                      comm, status);
   WTIME(TV2);

   PMPI_Type_size(stype, &sbytes);
   sbytes = scount * sbytes;

   PMPI_Type_size(rtype, &rbytes);
   rbytes = rcount * rbytes;

   bytes = 0;
   if (dest != MPI_PROC_NULL) bytes += sbytes;
   if (src  != MPI_PROC_NULL) bytes += rbytes;

   nvtxRangeEnd(range);
   LogEvent(SENDRECV_ID, TV1, TV2, src, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Sendrecv_replace                   */
/*----------------------------------------------------------*/
int MPI_Sendrecv_replace(void * buf, int count, MPI_Datatype type, int dest, int stag,
                         int src, int rtag, MPI_Comm comm, MPI_Status * status)
{
   int rc, bytes, sbytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SENDRECV_REPLACE_ID]);

   WTIME(TV1);
   rc = PMPI_Sendrecv_replace(buf, count, type, dest, stag,
                              src, rtag, comm, status);
   WTIME(TV2);

   PMPI_Type_size(type, &sbytes);
   sbytes = count * sbytes;

   bytes = 0;
   if (dest != MPI_PROC_NULL) bytes += sbytes;
   if (src  != MPI_PROC_NULL) bytes += sbytes;

   nvtxRangeEnd(range);
   LogEvent(SENDRECV_REPLACE_ID, TV1, TV2, src, dest, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Buffer_attach                      */
/*----------------------------------------------------------*/
int MPI_Buffer_attach(void * buffer, int size)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BUFFER_ATTACH_ID]);

   WTIME(TV1);
   rc = PMPI_Buffer_attach(buffer, size);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(BUFFER_ATTACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Buffer_detach                      */
/*----------------------------------------------------------*/
int MPI_Buffer_detach(void * buffer, int * size)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BUFFER_DETACH_ID]);

   WTIME(TV1);
   rc = PMPI_Buffer_detach(buffer, size);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(BUFFER_DETACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Probe                              */
/*----------------------------------------------------------*/
int MPI_Probe(int src, int tag, MPI_Comm comm, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[PROBE_ID]);

   WTIME(TV1);
   rc = PMPI_Probe(src, tag, comm, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(PROBE_ID, TV1, TV2, src, -1, -1, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iprobe                             */
/*----------------------------------------------------------*/
int MPI_Iprobe(int src, int tag, MPI_Comm comm, int * flag, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IPROBE_ID]);

   WTIME(TV1);
   rc = PMPI_Iprobe(src, tag, comm, flag, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(IPROBE_ID, TV1, TV2, src, -1, -1, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Test                               */
/*----------------------------------------------------------*/
int MPI_Test(MPI_Request * request, int * flag, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TEST_ID]);

   WTIME(TV1);
   rc = PMPI_Test(request, flag, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TEST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Testany                            */
/*----------------------------------------------------------*/
int MPI_Testany(int num, MPI_Request * req, int * indx, int * flag, 
                MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTANY_ID]);

   WTIME(TV1);
   rc = PMPI_Testany(num, req, indx, flag, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTANY_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Testall                            */
/*----------------------------------------------------------*/
int MPI_Testall(int num, MPI_Request * req, int * flag, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTALL_ID]);

   WTIME(TV1);
   rc = PMPI_Testall(num, req, flag, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Testsome                           */
/*----------------------------------------------------------*/
int MPI_Testsome(int inum, MPI_Request * req, int * onum, int * ind, 
                 MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTSOME_ID]);

   WTIME(TV1);
   rc = PMPI_Testsome(inum, req, onum, ind, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTSOME_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Wait                               */
/*----------------------------------------------------------*/
int MPI_Wait(MPI_Request * request, MPI_Status * status)
{
   int rc, src;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAIT_ID]);

   WTIME(TV1);
   rc = PMPI_Wait(request, status);
   WTIME(TV2);

   /* note: status is only filled-out for recv requests */
   if (status != MPI_STATUS_IGNORE)
       src   = status->MPI_SOURCE;
   else
       src = -1;

   nvtxRangeEnd(range);
   LogEvent(WAIT_ID, TV1, TV2, src, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Waitany                            */
/*----------------------------------------------------------*/
int MPI_Waitany(int num, MPI_Request * req, int * indx, MPI_Status * status)
{
   int rc, src;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITANY_ID]);

   WTIME(TV1);
   rc = PMPI_Waitany(num, req, indx, status);
   WTIME(TV2);

   /* note: status is only filled-out for recv requests */
   if (status != MPI_STATUS_IGNORE)
       src   = status->MPI_SOURCE;
   else
       src = -1;

   nvtxRangeEnd(range);
   LogEvent(WAITANY_ID, TV1, TV2, src, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Waitall                            */
/*----------------------------------------------------------*/
int MPI_Waitall(int num, MPI_Request * req, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITALL_ID]);

   WTIME(TV1);
   rc = PMPI_Waitall(num, req, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WAITALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Waitsome                           */
/*----------------------------------------------------------*/
int MPI_Waitsome(int inum, MPI_Request * req, int * onum, int * ind, 
                 MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITSOME_ID]);

   WTIME(TV1);
   rc = PMPI_Waitsome(inum, req, onum, ind, status);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WAITSOME_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Start                              */
/*----------------------------------------------------------*/
int MPI_Start(MPI_Request * req)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[START_ID]);

   WTIME(TV1);
   rc = PMPI_Start(req);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(START_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Startall                           */
/*----------------------------------------------------------*/
int MPI_Startall(int num, MPI_Request * req)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[STARTALL_ID]);

   WTIME(TV1);
   rc = PMPI_Startall(num, req);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(STARTALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Bcast                              */
/*----------------------------------------------------------*/
int MPI_Bcast(void * data, int count, MPI_Datatype type, 
              int root, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BCAST_ID]);

   if (barrier_flag[BCAST_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Bcast(data, count, type, root, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(BCAST_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ibcast                             */
/*----------------------------------------------------------*/
int MPI_Ibcast(void * data, int count, MPI_Datatype type, 
               int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBCAST_ID]);

   WTIME(TV1);
   rc = PMPI_Ibcast(data, count, type, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(IBCAST_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Barrier                            */
/*----------------------------------------------------------*/
int MPI_Barrier(MPI_Comm comm)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BARRIER_ID]);

   WTIME(TV1);
   rc = PMPI_Barrier(comm);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(BARRIER_ID, TV1, TV2, -1, -1, -1, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ibarrier                           */
/*----------------------------------------------------------*/
int MPI_Ibarrier(MPI_Comm comm, MPI_Request * req)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBARRIER_ID]);

   WTIME(TV1);
   rc = PMPI_Ibarrier(comm, req);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(IBARRIER_ID, TV1, TV2, -1, -1, -1, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Reduce                             */
/*----------------------------------------------------------*/
int MPI_Reduce(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
               MPI_Op op, int root, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_ID]);

   if (barrier_flag[REDUCE_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Reduce(sbuf, rbuf, count, type, op, root, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(REDUCE_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ireduce                            */
/*----------------------------------------------------------*/
int MPI_Ireduce(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                MPI_Op op, int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_ID]);

   WTIME(TV1);
   rc = PMPI_Ireduce(sbuf, rbuf, count, type, op, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(IREDUCE_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Allreduce                          */
/*----------------------------------------------------------*/
int MPI_Allreduce(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                  MPI_Op op, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLREDUCE_ID]);

   if (barrier_flag[ALLREDUCE_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Allreduce(sbuf, rbuf, count, type, op, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(ALLREDUCE_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iallreduce                         */
/*----------------------------------------------------------*/
int MPI_Iallreduce(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                   MPI_Op op, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLREDUCE_ID]);

   WTIME(TV1);
   rc = PMPI_Iallreduce(sbuf, rbuf, count, type, op, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(IALLREDUCE_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Reduce_scatter                     */
/*----------------------------------------------------------*/
int MPI_Reduce_scatter(sbuf_t sbuf, void * rbuf, iarray_t counts, MPI_Datatype type, 
                       MPI_Op op, MPI_Comm comm)
{
   int rc, i, bytes, num, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_SCATTER_ID]);

   if (barrier_flag[REDUCE_SCATTER_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Reduce_scatter(sbuf, rbuf, counts, type, op, comm);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(type, &bytes);

   num = 0;
   for (i=0; i<tasks; i++) num += counts[i];
   bytes = num * bytes;

   nvtxRangeEnd(range);
   LogEvent(REDUCE_SCATTER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ireduce_scatter                    */
/*----------------------------------------------------------*/
int MPI_Ireduce_scatter(sbuf_t sbuf, void * rbuf, iarray_t counts, MPI_Datatype type, 
                        MPI_Op op, MPI_Comm comm, MPI_Request * req)
{
   int rc, i, bytes, num, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_SCATTER_ID]);

   WTIME(TV1);
   rc = PMPI_Ireduce_scatter(sbuf, rbuf, counts, type, op, comm, req);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(type, &bytes);

   num = 0;
   for (i=0; i<tasks; i++) num += counts[i];
   bytes = num * bytes;

   nvtxRangeEnd(range);
   LogEvent(IREDUCE_SCATTER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Reduce_scatter_block               */
/*----------------------------------------------------------*/
int MPI_Reduce_scatter_block(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                             MPI_Op op, MPI_Comm comm)
{
   int rc, i, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_SCATTER_BLOCK_ID]);

   if (barrier_flag[REDUCE_SCATTER_BLOCK_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Reduce_scatter_block(sbuf, rbuf, count, type, op, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);

   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(REDUCE_SCATTER_BLOCK_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ireduce_scatter_block              */
/*----------------------------------------------------------*/
int MPI_Ireduce_scatter_block(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                              MPI_Op op, MPI_Comm comm, MPI_Request * req)
{
   int rc, i, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_SCATTER_BLOCK_ID]);

   WTIME(TV1);
   rc = PMPI_Ireduce_scatter_block(sbuf, rbuf, count, type, op, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);

   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(IREDUCE_SCATTER_BLOCK_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Gather                             */
/*----------------------------------------------------------*/
int MPI_Gather(sbuf_t sbuf, int scount, MPI_Datatype stype,
               void * rbuf, int rcount, MPI_Datatype rtype,
               int root, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GATHER_ID]);

   if (barrier_flag[GATHER_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Gather(sbuf, scount, stype, rbuf, rcount, rtype, root, comm);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(GATHER_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Igather                            */
/*----------------------------------------------------------*/
int MPI_Igather(sbuf_t sbuf, int scount, MPI_Datatype stype,
                void * rbuf, int rcount, MPI_Datatype rtype,
                int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IGATHER_ID]);

   WTIME(TV1);
   rc = PMPI_Igather(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(IGATHER_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Gatherv                            */
/*----------------------------------------------------------*/
int MPI_Gatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                int root, MPI_Comm comm)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GATHERV_ID]);

   if (barrier_flag[GATHERV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Gatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, root, comm);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(GATHERV_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Igatherv                           */
/*----------------------------------------------------------*/
int MPI_Igatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                 void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                 int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IGATHERV_ID]);

   WTIME(TV1);
   rc = PMPI_Igatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(IGATHERV_ID, TV1, TV2, -1, root, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Scan                               */
/*----------------------------------------------------------*/
int MPI_Scan(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
             MPI_Op op, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCAN_ID]);

   if (barrier_flag[SCAN_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Scan(sbuf, rbuf, count, type, op, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(SCAN_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iscan                              */
/*----------------------------------------------------------*/
int MPI_Iscan(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
              MPI_Op op, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCAN_ID]);

   WTIME(TV1);
   rc = PMPI_Iscan(sbuf, rbuf, count, type, op, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(ISCAN_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Exscan                             */
/*----------------------------------------------------------*/
int MPI_Exscan(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
               MPI_Op op, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[EXSCAN_ID]);

   if (barrier_flag[EXSCAN_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Exscan(sbuf, rbuf, count, type, op, comm);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(EXSCAN_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iexscan                            */
/*----------------------------------------------------------*/
int MPI_Iexscan(sbuf_t sbuf, void * rbuf, int count, MPI_Datatype type, 
                MPI_Op op, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IEXSCAN_ID]);

   WTIME(TV1);
   rc = PMPI_Iexscan(sbuf, rbuf, count, type, op, comm, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   nvtxRangeEnd(range);
   LogEvent(IEXSCAN_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Allgather                          */
/*----------------------------------------------------------*/
int MPI_Allgather(sbuf_t sbuf, int scount, MPI_Datatype stype,
                  void * rbuf, int rcount, MPI_Datatype rtype,
                  MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLGATHER_ID]);

   if (barrier_flag[ALLGATHER_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(ALLGATHER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Neighbor_allgather                 */
/*----------------------------------------------------------*/
int MPI_Neighbor_allgather(sbuf_t sbuf, int scount, MPI_Datatype stype,
                           void * rbuf, int rcount, MPI_Datatype rtype,
                           MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLGATHER_ID]);

   if (barrier_flag[NEIGHBOR_ALLGATHER_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Neighbor_allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLGATHER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iallgather                         */
/*----------------------------------------------------------*/
int MPI_Iallgather(sbuf_t sbuf, int scount, MPI_Datatype stype,
                   void * rbuf, int rcount, MPI_Datatype rtype,
                   MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLGATHER_ID]);

   WTIME(TV1);
   rc = PMPI_Iallgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, req);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(IALLGATHER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ineighbor_allgather                */
/*----------------------------------------------------------*/
int MPI_Ineighbor_allgather(sbuf_t sbuf, int scount, MPI_Datatype stype,
                            void * rbuf, int rcount, MPI_Datatype rtype,
                            MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLGATHER_ID]);

   WTIME(TV1);
   rc = PMPI_Ineighbor_allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, req);
   WTIME(TV2);

   PMPI_Type_size(rtype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLGATHER_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Allgatherv                         */
/*----------------------------------------------------------*/
int MPI_Allgatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                   void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                   MPI_Comm comm)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLGATHERV_ID]);

   if (barrier_flag[ALLGATHERV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm);
   WTIME(TV2);

   PMPI_Comm_rank(comm, &id);
   PMPI_Type_size(rtype, &bytes);
   bytes = rcounts[id] * bytes;

   nvtxRangeEnd(range);
   LogEvent(ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Neighbor_allgatherv                */
/*----------------------------------------------------------*/
int MPI_Neighbor_allgatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                            void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                            MPI_Comm comm)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLGATHERV_ID]);

   if (barrier_flag[NEIGHBOR_ALLGATHERV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Neighbor_allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm);
   WTIME(TV2);

   PMPI_Comm_rank(comm, &id);
   PMPI_Type_size(rtype, &bytes);
   bytes = rcounts[id] * bytes;

   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iallgatherv                        */
/*----------------------------------------------------------*/
int MPI_Iallgatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                    void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                    MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLGATHERV_ID]);

   WTIME(TV1);
   rc = PMPI_Iallgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_rank(comm, &id);
   PMPI_Type_size(rtype, &bytes);
   bytes = rcounts[id] * bytes;

   nvtxRangeEnd(range);
   LogEvent(IALLGATHERV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ineighbor_allgatherv               */
/*----------------------------------------------------------*/
int MPI_Ineighbor_allgatherv(sbuf_t sbuf, int scount, MPI_Datatype stype,
                             void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                             MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLGATHERV_ID]);

   WTIME(TV1);
   rc = PMPI_Ineighbor_allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_rank(comm, &id);
   PMPI_Type_size(rtype, &bytes);
   bytes = rcounts[id] * bytes;

   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Scatter                            */
/*----------------------------------------------------------*/
int MPI_Scatter(sbuf_t sbuf, int scount, MPI_Datatype stype,
                void * rbuf, int rcount, MPI_Datatype rtype,
                int root, MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCATTER_ID]);

   if (barrier_flag[SCATTER_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Scatter(sbuf, scount, stype, rbuf, rcount, rtype, root, comm);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(SCATTER_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iscatter                           */
/*----------------------------------------------------------*/
int MPI_Iscatter(sbuf_t sbuf, int scount, MPI_Datatype stype,
                 void * rbuf, int rcount, MPI_Datatype rtype,
                 int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCATTER_ID]);

   WTIME(TV1);
   rc = PMPI_Iscatter(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(ISCATTER_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Scatterv                           */
/*----------------------------------------------------------*/
int MPI_Scatterv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                 void * rbuf, int rcount, MPI_Datatype rtype,
                 int root, MPI_Comm comm)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCATTERV_ID]);

   if (barrier_flag[SCATTERV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Scatterv(sbuf, scounts, sdisp, stype, rbuf, rcount, rtype, root, comm);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(SCATTERV_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Iscatterv                          */
/*----------------------------------------------------------*/
int MPI_Iscatterv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                  void * rbuf, int rcount, MPI_Datatype rtype,
                  int root, MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCATTERV_ID]);

   WTIME(TV1);
   rc = PMPI_Iscatterv(sbuf, scounts, sdisp, stype, rbuf, rcount, rtype, root, comm, req);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = rcount * bytes;

   nvtxRangeEnd(range);
   LogEvent(ISCATTERV_ID, TV1, TV2, root, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Alltoall                           */
/*----------------------------------------------------------*/
int MPI_Alltoall(sbuf_t sbuf, int scount, MPI_Datatype stype,
                 void * rbuf, int rcount, MPI_Datatype rtype,
                 MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALL_ID]);

   if (barrier_flag[ALLTOALL_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(ALLTOALL_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Neighbor_alltoall                  */
/*----------------------------------------------------------*/
int MPI_Neighbor_alltoall(sbuf_t sbuf, int scount, MPI_Datatype stype,
                          void * rbuf, int rcount, MPI_Datatype rtype,
                          MPI_Comm comm)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALL_ID]);

   if (barrier_flag[NEIGHBOR_ALLTOALL_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Neighbor_alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALL_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ialltoall                          */
/*----------------------------------------------------------*/
int MPI_Ialltoall(sbuf_t sbuf, int scount, MPI_Datatype stype,
                  void * rbuf, int rcount, MPI_Datatype rtype,
                  MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALL_ID]);

   WTIME(TV1);
   rc = PMPI_Ialltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, req);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(IALLTOALL_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ineighbor_alltoall                 */
/*----------------------------------------------------------*/
int MPI_Ineighbor_alltoall(sbuf_t sbuf, int scount, MPI_Datatype stype,
                           void * rbuf, int rcount, MPI_Datatype rtype,
                           MPI_Comm comm, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALL_ID]);

   WTIME(TV1);
   rc = PMPI_Ineighbor_alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, req);
   WTIME(TV2);

   PMPI_Type_size(stype, &bytes);
   bytes = scount * bytes;

   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALL_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Alltoallv                          */
/*----------------------------------------------------------*/
int MPI_Alltoallv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                  void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                  MPI_Comm comm)
{
   int rc, i, count, bytes, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALLV_ID]);

   if (barrier_flag[ALLTOALLV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, 
                       rtype, comm);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(stype, &bytes);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * bytes) / tasks;

   nvtxRangeEnd(range);
   LogEvent(ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Neighbor_alltoallv                 */
/*----------------------------------------------------------*/
int MPI_Neighbor_alltoallv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                           void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                           MPI_Comm comm)
{
   int rc, i, count, bytes, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALLV_ID]);

   if (barrier_flag[NEIGHBOR_ALLTOALLV_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Neighbor_alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(stype, &bytes);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * bytes) / tasks;

   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ialltoallv                         */
/*----------------------------------------------------------*/
int MPI_Ialltoallv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                   void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                   MPI_Comm comm, MPI_Request * req)
{
   int rc, i, count, bytes, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALLV_ID]);

   WTIME(TV1);
   rc = PMPI_Ialltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(stype, &bytes);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * bytes) / tasks;

   nvtxRangeEnd(range);
   LogEvent(IALLTOALLV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ineighbor_alltoallv                */
/*----------------------------------------------------------*/
int MPI_Ineighbor_alltoallv(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, MPI_Datatype stype,
                            void * rbuf, iarray_t rcounts, iarray_t rdisp, MPI_Datatype rtype,
                            MPI_Comm comm, MPI_Request * req)
{
   int rc, i, count, bytes, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALLV_ID]);

   WTIME(TV1);
   rc = PMPI_Ineighbor_alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);
   PMPI_Type_size(stype, &bytes);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * bytes) / tasks;

   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, comm); 
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Alltoallw                          */
/*----------------------------------------------------------*/
int MPI_Alltoallw(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, datarray_t stype,
                  void * rbuf, iarray_t rcounts, iarray_t rdisp, datarray_t rtype,
                  MPI_Comm comm)
{
   int rc, i, count, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALLW_ID]);

   if (barrier_flag[ALLTOALLW_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp,
                       rtype, comm);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      PMPI_Type_size(stype[i], &sz);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   nvtxRangeEnd(range);
   LogEvent(ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Neighbor_alltoallw                 */
/*----------------------------------------------------------*/
int MPI_Neighbor_alltoallw(sbuf_t sbuf, iarray_t scounts, const MPI_Aint * sdisp, datarray_t stype,
                           void * rbuf, iarray_t rcounts, const MPI_Aint * rdisp, datarray_t rtype,
                           MPI_Comm comm)
{
   int rc, i, count, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALLW_ID]);

   if (barrier_flag[NEIGHBOR_ALLTOALLW_ID]) 
   {
      WTIME(TV1);
      rc = PMPI_Barrier(comm);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   rc = PMPI_Neighbor_alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      PMPI_Type_size(stype[i], &sz);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ialltoallw                         */
/*----------------------------------------------------------*/
int MPI_Ialltoallw(sbuf_t sbuf, iarray_t scounts, iarray_t sdisp, datarray_t stype,
                   void * rbuf, iarray_t rcounts, iarray_t rdisp, datarray_t rtype,
                   MPI_Comm comm, MPI_Request * req)
{
   int rc, i, count, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALLW_ID]);

   WTIME(TV1);
   rc = PMPI_Ialltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      PMPI_Type_size(stype[i], &sz);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   nvtxRangeEnd(range);
   LogEvent(IALLTOALLW_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Ineighbor_alltoallw                */
/*----------------------------------------------------------*/
int MPI_Ineighbor_alltoallw(sbuf_t sbuf, iarray_t scounts, const MPI_Aint * sdisp, datarray_t stype,
                            void * rbuf, iarray_t rcounts, const MPI_Aint * rdisp, datarray_t rtype,
                            MPI_Comm comm, MPI_Request * req)
{
   int rc, i, count, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALLW_ID]);

   WTIME(TV1);
   rc = PMPI_Ineighbor_alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req);
   WTIME(TV2);

   PMPI_Comm_size(comm, &tasks);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      PMPI_Type_size(stype[i], &sz);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Accumulate                         */
/*----------------------------------------------------------*/
int MPI_Accumulate(const void * origin_addr, int origin_count, MPI_Datatype origin_datatype,
                   int target_rank, MPI_Aint target_disp, int target_count, 
                   MPI_Datatype target_datatype, MPI_Op op, MPI_Win win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ACCUMULATE_ID]);

   WTIME(TV1);
   rc = PMPI_Accumulate(origin_addr, origin_count, origin_datatype, target_rank,
                        target_disp, target_count, target_datatype, op, win);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(ACCUMULATE_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Fetch_and_op                       */
/*----------------------------------------------------------*/
int MPI_Fetch_and_op(const void * origin_addr, void * result_addr, MPI_Datatype datatype,
                     int target_rank, MPI_Aint target_disp, MPI_Op op, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FETCH_AND_OP_ID]);

   WTIME(TV1);
   rc = PMPI_Fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(FETCH_AND_OP_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Get                                */
/*----------------------------------------------------------*/
int MPI_Get(void * origin_addr, int origin_count, MPI_Datatype origin_datatype,
            int target_rank, MPI_Aint target_disp, int target_count, 
            MPI_Datatype target_datatype, MPI_Win win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GET_ID]);
 
   WTIME(TV1);
   rc = PMPI_Get(origin_addr, origin_count, origin_datatype, target_rank,
                 target_disp, target_count, target_datatype, win);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(GET_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Get_accumulate                     */
/*----------------------------------------------------------*/
int MPI_Get_accumulate(const void *origin_addr, int origin_count,
        MPI_Datatype origin_datatype, void *result_addr, int result_count,
        MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp,
        int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GET_ACCUMULATE_ID]);

   WTIME(TV1);
   rc = PMPI_Get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count,
                            result_datatype, target_rank, target_disp, target_count, target_datatype, op, win);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(GET_ACCUMULATE_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Put                                */
/*----------------------------------------------------------*/
int MPI_Put(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
            int target_rank, MPI_Aint target_disp, int target_count, 
            MPI_Datatype target_datatype, MPI_Win win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[PUT_ID]);

   WTIME(TV1);
   rc = PMPI_Put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, 
                 target_count, target_datatype, win);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(PUT_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Raccumulate                        */
/*----------------------------------------------------------*/
int MPI_Raccumulate(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype,
                    int target_rank, MPI_Aint target_disp, int target_count, 
                    MPI_Datatype target_datatype, MPI_Op op, MPI_Win win, MPI_Request *request)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RACCUMULATE_ID]);

   WTIME(TV1);
   rc = PMPI_Raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
                         target_count, target_datatype, op, win, request);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(RACCUMULATE_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Rget                               */
/*----------------------------------------------------------*/
int MPI_Rget(void *origin_addr, int origin_count, MPI_Datatype origin_datatype,
             int target_rank, MPI_Aint target_disp, int target_count, 
             MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RGET_ID]);

   WTIME(TV1);
   rc = PMPI_Rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
                  target_count, target_datatype, win, request);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(RGET_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Rget_accumulate                    */
/*----------------------------------------------------------*/
int MPI_Rget_accumulate(const void *origin_addr, int origin_count,
        MPI_Datatype origin_datatype, void *result_addr, int result_count,
        MPI_Datatype result_datatype, int target_rank, MPI_Aint target_disp,
        int target_count, MPI_Datatype target_datatype, MPI_Op op, MPI_Win win,
        MPI_Request *request)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RGET_ACCUMULATE_ID]);

   WTIME(TV1);
   rc = PMPI_Rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr,
                             result_count, result_datatype, target_rank, target_disp,
                             target_count, target_datatype, op, win, request);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(RGET_ACCUMULATE_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Rput                               */
/*----------------------------------------------------------*/
int MPI_Rput(const void *origin_addr, int origin_count, MPI_Datatype origin_datatype, 
             int target_rank, MPI_Aint target_disp, int target_count, 
             MPI_Datatype target_datatype, MPI_Win win, MPI_Request *request)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RPUT_ID]);

   WTIME(TV1);
   rc = PMPI_Rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
                  target_count, target_datatype, win, request);
   WTIME(TV2);

   PMPI_Type_size(origin_datatype, &bytes);
   bytes = origin_count*bytes;

   nvtxRangeEnd(range);
   LogEvent(RPUT_ID, TV1, TV2, -1, target_rank, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_allocate                       */
/*----------------------------------------------------------*/
int MPI_Win_allocate(MPI_Aint size, int disp_unit, MPI_Info info, 
                     MPI_Comm comm, void *baseptr, MPI_Win *win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ALLOCATE_ID]);

   WTIME(TV1);
   rc = PMPI_Win_allocate(size, disp_unit, info, comm, baseptr, win);
   WTIME(TV2);

   bytes = size;

   nvtxRangeEnd(range);
   LogEvent(WIN_ALLOCATE_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_allocate_shared                */
/*----------------------------------------------------------*/
int MPI_Win_allocate_shared(MPI_Aint size, int disp_unit, MPI_Info info, 
                            MPI_Comm comm, void *baseptr, MPI_Win *win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ALLOCATE_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_Win_allocate_shared(size, disp_unit, info, comm, baseptr, win);
   WTIME(TV2);

   bytes = size;

   nvtxRangeEnd(range);
   LogEvent(WIN_ALLOCATE_SHARED_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_attach                         */
/*----------------------------------------------------------*/
int MPI_Win_attach(MPI_Win win, void *base, MPI_Aint size)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ATTACH_ID]);

   WTIME(TV1);
   rc = PMPI_Win_attach(win, base, size);
   WTIME(TV2);

   bytes = size;

   nvtxRangeEnd(range);
   LogEvent(WIN_ATTACH_ID, TV1, TV2, -1, -1, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_complete                       */
/*----------------------------------------------------------*/
int MPI_Win_complete(MPI_Win win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_COMPLETE_ID]);

   WTIME(TV1);
   rc = PMPI_Win_complete(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_COMPLETE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_create                         */
/*----------------------------------------------------------*/
int MPI_Win_create(void *base, MPI_Aint size, int disp_unit, MPI_Info info, 
                   MPI_Comm comm, MPI_Win *win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_CREATE_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_create(base, size, disp_unit, info, comm, win);
   WTIME(TV2);
 
   bytes = size;

   nvtxRangeEnd(range);
   LogEvent(WIN_CREATE_ID, TV1, TV2, -1, -1, bytes, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_create_dynamic                 */
/*----------------------------------------------------------*/
int MPI_Win_create_dynamic(MPI_Info info, MPI_Comm comm, MPI_Win *win)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_CREATE_DYNAMIC_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_create_dynamic(info, comm, win);
   WTIME(TV2);
 
   nvtxRangeEnd(range);
   LogEvent(WIN_CREATE_DYNAMIC_ID, TV1, TV2, -1, -1, -1, comm);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_detach                         */
/*----------------------------------------------------------*/
int MPI_Win_detach(MPI_Win win, const void *base)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_DETACH_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_detach(win, base);
   WTIME(TV2);
 
   nvtxRangeEnd(range);
   LogEvent(WIN_DETACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_fence                          */
/*----------------------------------------------------------*/
int MPI_Win_fence(int assert, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FENCE_ID]);

   WTIME(TV1);
   rc = PMPI_Win_fence(assert, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FENCE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_flush                          */
/*----------------------------------------------------------*/
int MPI_Win_flush(int rank, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_ID]);

   WTIME(TV1);
   rc = PMPI_Win_flush(rank, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_flush_all                      */
/*----------------------------------------------------------*/
int MPI_Win_flush_all(MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_Win_flush_all(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_flush_local                    */
/*----------------------------------------------------------*/
int MPI_Win_flush_local(int rank, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_LOCAL_ID]);

   WTIME(TV1);
   rc = PMPI_Win_flush_local(rank, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_LOCAL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_flush_local_all                */
/*----------------------------------------------------------*/
int MPI_Win_flush_local_all(MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_LOCAL_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_Win_flush_local_all(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_LOCAL_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_free                           */
/*----------------------------------------------------------*/
int MPI_Win_free(MPI_Win *win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FREE_ID]);

   WTIME(TV1);
   rc = PMPI_Win_free(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FREE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_lock                           */
/*----------------------------------------------------------*/
int MPI_Win_lock(int lock_type, int rank, int assert, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_LOCK_ID]);

   WTIME(TV1);
   rc = PMPI_Win_lock(lock_type, rank, assert, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_LOCK_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_lock_all                       */
/*----------------------------------------------------------*/
int MPI_Win_lock_all(int assert, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_LOCK_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_Win_lock_all(assert, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_LOCK_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_post                           */
/*----------------------------------------------------------*/
int MPI_Win_post(MPI_Group group, int assert, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_POST_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_post(group, assert, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_POST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_start                          */
/*----------------------------------------------------------*/
int MPI_Win_start(MPI_Group group, int assert, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_START_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_start(group, assert, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_START_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_sync                           */
/*----------------------------------------------------------*/
int MPI_Win_sync(MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_SYNC_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_sync(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_SYNC_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_test                           */
/*----------------------------------------------------------*/
int MPI_Win_test(MPI_Win win, int *flag)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_TEST_ID]);
 
   WTIME(TV1);
   rc = PMPI_Win_test(win, flag);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_TEST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_unlock                         */
/*----------------------------------------------------------*/
int MPI_Win_unlock(int rank, MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_UNLOCK_ID]);

   WTIME(TV1);
   rc = PMPI_Win_unlock(rank, win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_UNLOCK_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_unlock_all                     */
/*----------------------------------------------------------*/
int MPI_Win_unlock_all(MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_UNLOCK_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_Win_unlock_all(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_UNLOCK_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Win_wait                           */
/*----------------------------------------------------------*/
int MPI_Win_wait(MPI_Win win)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_WAIT_ID]);

   WTIME(TV1);
   rc = PMPI_Win_wait(win);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_WAIT_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return rc;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_close                         */
/*----------------------------------------------------------*/
int MPI_File_close(MPI_File * fh)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_CLOSE_ID]);

   WTIME(TV1);
   rc = PMPI_File_close(fh);
   WTIME(TV2);

   LogIOEvent(FILE_CLOSE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_delete                        */
/*----------------------------------------------------------*/
int MPI_File_delete(name_t filename, MPI_Info info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_DELETE_ID]);

   WTIME(TV1);
   rc = PMPI_File_delete(filename, info);
   WTIME(TV2);

   LogIOEvent(FILE_DELETE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iread                         */
/*----------------------------------------------------------*/
int MPI_File_iread(MPI_File fh, void * buf, int count, 
                   MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_ID]);

   WTIME(TV1);
   rc = PMPI_File_iread(fh, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_IREAD_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iread_at                      */
/*----------------------------------------------------------*/
int MPI_File_iread_at(MPI_File fh, MPI_Offset offset, void * buf, int count, 
                      MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_AT_ID]);

   WTIME(TV1);
   rc = PMPI_File_iread_at(fh, offset, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_IREAD_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iread_shared                  */
/*----------------------------------------------------------*/
int MPI_File_iread_shared(MPI_File fh, void * buf, int count, 
                          MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_File_iread_shared(fh, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_IREAD_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iwrite                        */
/*----------------------------------------------------------*/
int MPI_File_iwrite(MPI_File fh, sbuf_t buf, int count, 
                    MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_ID]);

   WTIME(TV1);
   rc = PMPI_File_iwrite(fh, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_IWRITE_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iwrite_at                     */
/*----------------------------------------------------------*/
int MPI_File_iwrite_at(MPI_File fh, MPI_Offset offset, sbuf_t buf, int count, 
                       MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_AT_ID]);

   WTIME(TV1);
   rc = PMPI_File_iwrite_at(fh, offset, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_IWRITE_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_iwrite_shared                 */
/*----------------------------------------------------------*/
int MPI_File_iwrite_shared(MPI_File fh, sbuf_t buf, int count, 
                           MPI_Datatype type, MPI_Request * req)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_File_iwrite_shared(fh, buf, count, type, req);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_IWRITE_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_open                          */
/*----------------------------------------------------------*/
int MPI_File_open(MPI_Comm comm, name_t filename, int mode, 
                  MPI_Info info, MPI_File * fh)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_OPEN_ID]);

   WTIME(TV1);
   rc = PMPI_File_open(comm, filename, mode, info, fh);
   WTIME(TV2);

   LogIOEvent(FILE_OPEN_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_preallocate                   */
/*----------------------------------------------------------*/
int MPI_File_preallocate(MPI_File fh, MPI_Offset size_bytes)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_PREALLOCATE_ID]);

   WTIME(TV1);
   rc = PMPI_File_preallocate(fh, size_bytes);
   WTIME(TV2);

   LogIOEvent(FILE_PREALLOCATE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read                          */
/*----------------------------------------------------------*/
int MPI_File_read(MPI_File fh, void * buf, int count, 
                  MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ID]);

   WTIME(TV1);
   rc = PMPI_File_read(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_all                      */
/*----------------------------------------------------------*/
int MPI_File_read_all(MPI_File fh, void * buf, int count, 
                      MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_all(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_ALL_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_all_begin                */
/*----------------------------------------------------------*/
int MPI_File_read_all_begin(MPI_File fh, void * buf, int count, 
                            MPI_Datatype type)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_all_begin(fh, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_ALL_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_all_end                  */
/*----------------------------------------------------------*/
int MPI_File_read_all_end(MPI_File fh, void * buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_all_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_READ_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_at                       */
/*----------------------------------------------------------*/
int MPI_File_read_at(MPI_File fh, MPI_Offset offset, void * buf, int count, 
                     MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_at(fh, offset, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_READ_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_at_all                   */
/*----------------------------------------------------------*/
int MPI_File_read_at_all(MPI_File fh, MPI_Offset offset, void * buf, int count, 
                         MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_at_all(fh, offset, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_READ_AT_ALL_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_at_all_begin             */
/*----------------------------------------------------------*/
int MPI_File_read_at_all_begin(MPI_File fh, MPI_Offset offset, void * buf, 
                               int count, MPI_Datatype type)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_at_all_begin(fh, offset, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_READ_AT_ALL_BEGIN_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_at_all_end               */
/*----------------------------------------------------------*/
int MPI_File_read_at_all_end(MPI_File fh, void * buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_at_all_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_READ_AT_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_ordered                  */
/*----------------------------------------------------------*/
int MPI_File_read_ordered(MPI_File fh, void * buf, int count, 
                          MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_ordered(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_ORDERED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_ordered_begin            */
/*----------------------------------------------------------*/
int MPI_File_read_ordered_begin(MPI_File fh, void * buf, int count, 
                                MPI_Datatype type)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_ordered_begin(fh, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_ORDERED_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_ordered_end              */
/*----------------------------------------------------------*/
int MPI_File_read_ordered_end(MPI_File fh, void * buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_ordered_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_READ_ORDERED_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_read_shared                   */
/*----------------------------------------------------------*/
int MPI_File_read_shared(MPI_File fh, void * buf, int count, 
                         MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_File_read_shared(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_READ_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_seek                          */
/*----------------------------------------------------------*/
int MPI_File_seek(MPI_File fh, MPI_Offset offset, int whence)
{
   int rc;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SEEK_ID]);

   WTIME(TV1);
   rc = PMPI_File_seek(fh, offset, whence);
   WTIME(TV2);

   loff = (long) offset;

   LogIOEvent(FILE_SEEK_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_seek_shared                   */
/*----------------------------------------------------------*/
int MPI_File_seek_shared(MPI_File fh, MPI_Offset offset, int whence)
{
   int rc;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SEEK_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_File_seek_shared(fh, offset, whence);
   WTIME(TV2);

   loff = (long) offset;

   LogIOEvent(FILE_SEEK_SHARED_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_set_view                      */
/*----------------------------------------------------------*/
int MPI_File_set_view(MPI_File fh, MPI_Offset offset, MPI_Datatype etype,
                      MPI_Datatype filetype, name_t datarep, MPI_Info info)
{
   int rc;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SET_VIEW_ID]);

   WTIME(TV1);
   rc = PMPI_File_set_view(fh, offset, etype, filetype, datarep, info);
   WTIME(TV2);

   loff = (long) offset;

   LogIOEvent(FILE_SET_VIEW_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_sync                          */
/*----------------------------------------------------------*/
int MPI_File_sync(MPI_File fh)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SYNC_ID]);

   WTIME(TV1);
   rc = PMPI_File_sync(fh);
   WTIME(TV2);

   LogIOEvent(FILE_SYNC_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write                         */
/*----------------------------------------------------------*/
int MPI_File_write(MPI_File fh, sbuf_t buf, int count, 
                   MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ID]);

   WTIME(TV1);
   rc = PMPI_File_write(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_all                     */
/*----------------------------------------------------------*/
int MPI_File_write_all(MPI_File fh, sbuf_t buf, int count, 
                       MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_all(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_ALL_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_all_begin               */
/*----------------------------------------------------------*/
int MPI_File_write_all_begin(MPI_File fh, sbuf_t buf, int count, 
                             MPI_Datatype type)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_all_begin(fh, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_ALL_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_all_end                 */
/*----------------------------------------------------------*/
int MPI_File_write_all_end(MPI_File fh, sbuf_t buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_all_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_at                      */
/*----------------------------------------------------------*/
int MPI_File_write_at(MPI_File fh, MPI_Offset offset, sbuf_t buf, int count, 
                      MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_at(fh, offset, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_WRITE_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_at_all                  */
/*----------------------------------------------------------*/
int MPI_File_write_at_all(MPI_File fh, MPI_Offset offset, sbuf_t buf, int count, 
                          MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_at_all(fh, offset, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_WRITE_AT_ALL_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_at_all_begin            */
/*----------------------------------------------------------*/
int MPI_File_write_at_all_begin(MPI_File fh, MPI_Offset offset, sbuf_t buf, 
                                int count, MPI_Datatype type)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_at_all_begin(fh, offset, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   loff = (long) offset;

   LogIOEvent(FILE_WRITE_AT_ALL_BEGIN_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_at_all_end              */
/*----------------------------------------------------------*/
int MPI_File_write_at_all_end(MPI_File fh, sbuf_t buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_at_all_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_AT_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_ordered                 */
/*----------------------------------------------------------*/
int MPI_File_write_ordered(MPI_File fh, sbuf_t buf, int count, 
                           MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_ordered(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_ORDERED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_ordered_begin           */
/*----------------------------------------------------------*/
int MPI_File_write_ordered_begin(MPI_File fh, sbuf_t buf, int count, 
                                 MPI_Datatype type)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_BEGIN_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_ordered_begin(fh, buf, count, type);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_ORDERED_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_ordered_end             */
/*----------------------------------------------------------*/
int MPI_File_write_ordered_end(MPI_File fh, sbuf_t buf, MPI_Status * status)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_END_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_ordered_end(fh, buf, status);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_ORDERED_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_File_write_shared                  */
/*----------------------------------------------------------*/
int MPI_File_write_shared(MPI_File fh, sbuf_t buf, int count, 
                          MPI_Datatype type, MPI_Status * status)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_SHARED_ID]);

   WTIME(TV1);
   rc = PMPI_File_write_shared(fh, buf, count, type, status);
   WTIME(TV2);

   PMPI_Type_size(type, &bytes);
   bytes = count * bytes;

   LogIOEvent(FILE_WRITE_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return rc;
}


/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Finalize                           */
/*----------------------------------------------------------*/
int MPI_Finalize(void)
{
   int rc;

   write_profile_data();

   rc = PMPI_Finalize();
   return rc;
}

/*----------------------------------------------------------*/
/*    wrapper for C: MPI_Abort                              */
/*----------------------------------------------------------*/
int MPI_Abort(MPI_Comm comm, int err)
{
   int rc;
   if (traceback_abort) mpitrace_traceback(&taskid);
   rc = PMPI_Abort(comm, err);
   return rc;
}

#ifndef SKIP_FORTRAN

/*----------------------------------------------------------*/
/*    fortran profiling entry points                        */
/*----------------------------------------------------------*/
#ifdef ADD_UNDERSCORE
#define pmpi_type_size                    pmpi_type_size_
#define pmpi_get_count                    pmpi_get_count_
#define pmpi_init                         pmpi_init_ 
#define pmpi_init_thread                  pmpi_init_thread_ 
#define pmpi_pcontrol                     pmpi_pcontrol_ 
#define pmpi_finalize                     pmpi_finalize_ 
#define pmpi_abort                        pmpi_abort_ 
#define pmpi_comm_rank                    pmpi_comm_rank_ 
#define pmpi_comm_size                    pmpi_comm_size_ 
#define pmpi_send                         pmpi_send_ 
#define pmpi_ssend                        pmpi_ssend_ 
#define pmpi_rsend                        pmpi_rsend_ 
#define pmpi_bsend                        pmpi_bsend_ 
#define pmpi_isend                        pmpi_isend_ 
#define pmpi_issend                       pmpi_issend_ 
#define pmpi_irsend                       pmpi_irsend_ 
#define pmpi_ibsend                       pmpi_ibsend_ 
#define pmpi_send_init                    pmpi_send_init_ 
#define pmpi_ssend_init                   pmpi_ssend_init_ 
#define pmpi_rsend_init                   pmpi_rsend_init_ 
#define pmpi_bsend_init                   pmpi_bsend_init_ 
#define pmpi_recv_init                    pmpi_recv_init_ 
#define pmpi_recv                         pmpi_recv_ 
#define pmpi_irecv                        pmpi_irecv_ 
#define pmpi_sendrecv                     pmpi_sendrecv_ 
#define pmpi_sendrecv_replace             pmpi_sendrecv_replace_ 
#define pmpi_buffer_attach                pmpi_buffer_attach_ 
#define pmpi_buffer_detach                pmpi_buffer_detach_ 
#define pmpi_probe                        pmpi_probe_ 
#define pmpi_iprobe                       pmpi_iprobe_ 
#define pmpi_test                         pmpi_test_ 
#define pmpi_testany                      pmpi_testany_ 
#define pmpi_testall                      pmpi_testall_ 
#define pmpi_testsome                     pmpi_testsome_ 
#define pmpi_wait                         pmpi_wait_ 
#define pmpi_waitany                      pmpi_waitany_ 
#define pmpi_waitall                      pmpi_waitall_ 
#define pmpi_waitsome                     pmpi_waitsome_ 
#define pmpi_start                        pmpi_start_ 
#define pmpi_startall                     pmpi_startall_ 
#define pmpi_bcast                        pmpi_bcast_ 
#define pmpi_barrier                      pmpi_barrier_ 
#define pmpi_reduce                       pmpi_reduce_ 
#define pmpi_allreduce                    pmpi_allreduce_ 
#define pmpi_reduce_scatter               pmpi_reduce_scatter_ 
#define pmpi_gather                       pmpi_gather_ 
#define pmpi_gatherv                      pmpi_gatherv_ 
#define pmpi_scan                         pmpi_scan_ 
#define pmpi_exscan                       pmpi_exscan_ 
#define pmpi_allgather                    pmpi_allgather_ 
#define pmpi_allgatherv                   pmpi_allgatherv_ 
#define pmpi_scatter                      pmpi_scatter_ 
#define pmpi_scatterv                     pmpi_scatterv_ 
#define pmpi_alltoall                     pmpi_alltoall_ 
#define pmpi_alltoallv                    pmpi_alltoallv_ 
#define pmpi_alltoallw                    pmpi_alltoallw_ 

#if MPI_VERSION >= 3
#define pmpi_ibcast                       pmpi_ibcast_ 
#define pmpi_ibarrier                     pmpi_ibarrier_ 
#define pmpi_ireduce                      pmpi_ireduce_ 
#define pmpi_iallreduce                   pmpi_iallreduce_ 
#define pmpi_ireduce_scatter              pmpi_ireduce_scatter_ 
#define pmpi_ireduce_scatter_block        pmpi_ireduce_scatter_block_ 
#define pmpi_igather                      pmpi_igather_ 
#define pmpi_igatherv                     pmpi_igatherv_ 
#define pmpi_iscan                        pmpi_iscan_ 
#define pmpi_iexscan                      pmpi_iexscan_ 
#define pmpi_iallgather                   pmpi_iallgather_ 
#define pmpi_neighbor_allgather           pmpi_neighbor_allgather_ 
#define pmpi_ineighbor_allgather          pmpi_ineighbor_allgather_ 
#define pmpi_iallgatherv                  pmpi_iallgatherv_ 
#define pmpi_neighbor_allgatherv          pmpi_neighbor_allgatherv_ 
#define pmpi_ineighbor_allgatherv         pmpi_ineighbor_allgatherv_ 
#define pmpi_iscatter                     pmpi_iscatter_ 
#define pmpi_iscatterv                    pmpi_iscatterv_ 
#define pmpi_ialltoall                    pmpi_ialltoall_ 
#define pmpi_neighbor_alltoall            pmpi_neighbor_alltoall_ 
#define pmpi_ineighbor_alltoall           pmpi_ineighbor_alltoall_ 
#define pmpi_ialltoallv                   pmpi_ialltoallv_ 
#define pmpi_neighbor_alltoallv           pmpi_neighbor_alltoallv_ 
#define pmpi_ineighbor_alltoallv          pmpi_ineighbor_alltoallv_ 
#define pmpi_ialltoallw                   pmpi_ialltoallw_ 
#define pmpi_neighbor_alltoallw           pmpi_neighbor_alltoallw_ 
#define pmpi_ineighbor_alltoallw          pmpi_ineighbor_alltoallw_ 
#define pmpi_accumulate                   pmpi_accumulate_ 
#define pmpi_fetch_and_op                 pmpi_fetch_and_op_ 
#define pmpi_get                          pmpi_get_ 
#define pmpi_get_accumulate               pmpi_get_accumulate_ 
#define pmpi_put                          pmpi_put_ 
#define pmpi_raccumulate                  pmpi_raccumulate_ 
#define pmpi_reduce_scatter_block         pmpi_reduce_scatter_block_ 
#define pmpi_rget                         pmpi_rget_ 
#define pmpi_rget_accumulate              pmpi_rget_accumulate_ 
#define pmpi_rput                         pmpi_rput_ 
#define pmpi_win_allocate                 pmpi_win_allocate_ 
#define pmpi_win_allocate_shared          pmpi_win_allocate_shared_ 
#define pmpi_win_attach                   pmpi_win_attach_ 
#define pmpi_win_complete                 pmpi_win_complete_ 
#define pmpi_win_create                   pmpi_win_create_ 
#define pmpi_win_create_dynamic           pmpi_win_create_dynamic_ 
#define pmpi_win_detach                   pmpi_win_detach_ 
#define pmpi_win_fence                    pmpi_win_fence_ 
#define pmpi_win_flush                    pmpi_win_flush_ 
#define pmpi_win_flush_all                pmpi_win_flush_all_ 
#define pmpi_win_flush_local              pmpi_win_flush_local_ 
#define pmpi_win_flush_local_all          pmpi_win_flush_local_all_ 
#define pmpi_win_free                     pmpi_win_free_ 
#define pmpi_win_lock                     pmpi_win_lock_ 
#define pmpi_win_lock_all                 pmpi_win_lock_all_ 
#define pmpi_win_post                     pmpi_win_post_ 
#define pmpi_win_start                    pmpi_win_start_ 
#define pmpi_win_sync                     pmpi_win_sync_ 
#define pmpi_win_test                     pmpi_win_test_ 
#define pmpi_win_unlock                   pmpi_win_unlock_ 
#define pmpi_win_unlock_all               pmpi_win_unlock_all_ 
#define pmpi_win_wait                     pmpi_win_wait_ 
#endif

#define pmpi_file_close                   pmpi_file_close_ 
#define pmpi_file_delete                  pmpi_file_delete_ 
#define pmpi_file_iread                   pmpi_file_iread_ 
#define pmpi_file_iread_at                pmpi_file_iread_at_ 
#define pmpi_file_iread_shared            pmpi_file_iread_shared_ 
#define pmpi_file_iwrite                  pmpi_file_iwrite_ 
#define pmpi_file_iwrite_at               pmpi_file_iwrite_at_ 
#define pmpi_file_iwrite_shared           pmpi_file_iwrite_shared_ 
#define pmpi_file_open                    pmpi_file_open_ 
#define pmpi_file_preallocate             pmpi_file_preallocate_ 
#define pmpi_file_read                    pmpi_file_read_ 
#define pmpi_file_read_all                pmpi_file_read_all_ 
#define pmpi_file_read_all_begin          pmpi_file_read_all_begin_ 
#define pmpi_file_read_all_end            pmpi_file_read_all_end_ 
#define pmpi_file_read_at                 pmpi_file_read_at_ 
#define pmpi_file_read_at_all             pmpi_file_read_at_all_ 
#define pmpi_file_read_at_all_begin       pmpi_file_read_at_all_begin_ 
#define pmpi_file_read_at_all_end         pmpi_file_read_at_all_end_ 
#define pmpi_file_read_ordered            pmpi_file_read_ordered_ 
#define pmpi_file_read_ordered_begin      pmpi_file_read_ordered_begin_ 
#define pmpi_file_read_ordered_end        pmpi_file_read_ordered_end_ 
#define pmpi_file_read_shared             pmpi_file_read_shared_ 
#define pmpi_file_seek                    pmpi_file_seek_ 
#define pmpi_file_seek_shared             pmpi_file_seek_shared_ 
#define pmpi_file_set_view                pmpi_file_set_view_ 
#define pmpi_file_sync                    pmpi_file_sync_ 
#define pmpi_file_write                   pmpi_file_write_ 
#define pmpi_file_write_all               pmpi_file_write_all_ 
#define pmpi_file_write_all_begin         pmpi_file_write_all_begin_ 
#define pmpi_file_write_all_end           pmpi_file_write_all_end_ 
#define pmpi_file_write_at                pmpi_file_write_at_ 
#define pmpi_file_write_at_all            pmpi_file_write_at_all_ 
#define pmpi_file_write_at_all_begin      pmpi_file_write_at_all_begin_ 
#define pmpi_file_write_at_all_end        pmpi_file_write_at_all_end_ 
#define pmpi_file_write_ordered           pmpi_file_write_ordered_ 
#define pmpi_file_write_ordered_begin     pmpi_file_write_ordered_begin_ 
#define pmpi_file_write_ordered_end       pmpi_file_write_ordered_end_ 
#define pmpi_file_write_shared            pmpi_file_write_shared_ 
#endif

void pmpi_init(int *);
#ifdef HAS_INIT_THREAD
void pmpi_init_thread(int *, int *, int *);
#endif
void pmpi_finalize(int *);
void pmpi_abort(int *, int *, int *);
void pmpi_comm_rank(int *, int *, int *);
void pmpi_comm_size(int *, int *, int *);
void pmpi_send(void *, int *, int *, int *, int *, int *, int *);
void pmpi_ssend(void *, int *, int *, int *, int *, int *, int *);
void pmpi_rsend(void *, int *, int *, int *, int *, int *, int *);
void pmpi_bsend(void *, int *, int *, int *, int *, int *, int *);
void pmpi_isend(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_issend(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_irsend(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_ibsend(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_send_init(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_ssend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_rsend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_bsend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_recv_init(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_recv(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_irecv(void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_sendrecv(void *, int *, int *, int *, int *, void *, int *, int *, int *, int *, int *, MPI_Status *, int *);
void pmpi_sendrecv_replace(void *, int *, int *, int *, int *, int *, int *, int *, MPI_Status *, int *);
void pmpi_buffer_attach(void *, int *, int *);
void pmpi_buffer_detach(void *, int *, int *);
void pmpi_probe(int *, int *, int *, MPI_Status *, int *);
void pmpi_iprobe(int *, int *, int *, int *, MPI_Status *, int *);
void pmpi_test(int *, int *, MPI_Status *, int *);
void pmpi_testany(int *, int *, int*, int *, MPI_Status *, int *);
void pmpi_testall(int *, int*, int *, MPI_Status *, int *);
void pmpi_testsome(int *, int *, int *, int *, MPI_Status *, int *);
void pmpi_wait(int *, MPI_Status *, int *);
void pmpi_waitany(int *, int *, int *, MPI_Status *, int *);
void pmpi_waitall(int *, int *, MPI_Status *, int *);
void pmpi_waitsome(int *, int *, int *, int *, MPI_Status *, int *);
void pmpi_start(int *, int *);
void pmpi_startall(int *, int *, int *);
void pmpi_bcast(void *, int *, int *, int *, int *, int *);
void pmpi_barrier(int *, int *);
void pmpi_reduce(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_allreduce(void *, void *, int *, int *, int *, int *, int *);
void pmpi_reduce_scatter(void *, void *, int *, int *, int *, int *, int *);
void pmpi_gather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_gatherv(void *, int *, int *, void *, int *, int *, int *,int *, int *, int *);
void pmpi_scan(void *, void *, int *, int *, int *, int *, int *);
void pmpi_exscan(void *, void *, int *, int *, int *, int *, int *);
void pmpi_allgather(void *, int *, int *, void *, int *, int *, int *, int *);
void pmpi_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_scatter(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_scatterv(void *, int *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_alltoall(void *, int *, int *, void *, int *, int *, int *, int *);
void pmpi_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void pmpi_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void pmpi_type_size(int *, int *, int *);
void pmpi_get_count(int *, int *, int *, int *);

#if MPI_VERSION >= 3
void pmpi_ibcast(void *, int *, int *, int *, int *, int *, int *);
void pmpi_ibarrier(int *, int *, int *);
void pmpi_ireduce(void *, void *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_iallreduce(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_ireduce_scatter(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_ireduce_scatter_block(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_igather(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_igatherv(void *, int *, int *, void *, int *, int *, int *, int *,int *, int *, int *);
void pmpi_iscan(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_iexscan(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_iallgather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_neighbor_allgather(void *, int *, int *, void *, int *, int *, int *, int *);
void pmpi_ineighbor_allgather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_iallgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_neighbor_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_ineighbor_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_iscatter(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_iscatterv(void *, int *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_ialltoall(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_neighbor_alltoall(void *, int *, int *, void *, int *, int *, int *, int *);
void pmpi_ineighbor_alltoall(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void pmpi_ialltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void pmpi_neighbor_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void pmpi_ineighbor_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void pmpi_ialltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void pmpi_neighbor_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void pmpi_ineighbor_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void pmpi_accumulate(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_fetch_and_op(void *, void *, int *, int *, int *, int *, int *, int *);
void pmpi_get(void *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_get_accumulate(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_put(void *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_raccumulate(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_reduce_scatter_block(void *, void *, int *, int *, int *, int *, int *);
void pmpi_rget(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_rget_accumulate(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_rput(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void pmpi_win_allocate(int *, int *, int *, int *, void *, int *, int *);
void pmpi_win_allocate_shared(int *, int *, int *, int *, void *, int *, int *);
void pmpi_win_attach(int *, void *, int *, int *);
void pmpi_win_complete(int *, int *);
void pmpi_win_create(void *, int *, int *, int *, int *, int *, int *);
void pmpi_win_create_dynamic(int *, int *, int *, int *);
void pmpi_win_detach(int *, void *, int *);
void pmpi_win_fence(int *, int *, int *);
void pmpi_win_flush(int *, int *, int *);
void pmpi_win_flush_all(int *, int *);
void pmpi_win_flush_local(int *, int *, int *);
void pmpi_win_flush_local_all(int *, int *);
void pmpi_win_free(int *, int *);
void pmpi_win_lock(int *, int *, int *, int *, int *);
void pmpi_win_lock_all(int *, int *, int *);
void pmpi_win_post(int *, int *, int *, int *);
void pmpi_win_start(int *, int *, int *, int *);
void pmpi_win_sync(int *, int *); 
void pmpi_win_test(int*, int *, int *); 
void pmpi_win_unlock(int *, int *, int *);
void pmpi_win_unlock_all(int *, int *);
void pmpi_win_wait(int *, int *);
#endif

/*-------------------------*/
/* Fortran MPI-IO Routines */
/*-------------------------*/
void pmpi_file_close(int * fh, int * err);
void pmpi_file_delete(char * filename, int * info, int * err, int charlen);
void pmpi_file_iread(int * fh, void * buf, int * count, int * type, int * req, int * err);
void pmpi_file_iread_at(int * fh, long long * offset, void * buf, int  * count, int * type, int * req, int * err);
void pmpi_file_iread_shared(int * fh, void * buf, int * count, int * type, int * req, int * err);
void pmpi_file_iwrite(int * fh, void * buf, int * count, int * type, int * req, int * err);
void pmpi_file_iwrite_at(int * fh, long long * offset, void * buf, int * count, int * type, int * req, int * err);
void pmpi_file_iwrite_shared(int * fh, void * buf, int * count, int * type, int * req, int * err);
void pmpi_file_open(int * comm, char * filename, int * mode, int * info, int * fh, int * err, int charlen);
void pmpi_file_preallocate(int * fh, long long * size_bytes, int * err);
void pmpi_file_read(int * fh, void * buf, int  * count, int * type, int * status, int * err);
void pmpi_file_read_all(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_read_all_begin(int* fh, void * buf, int * count, int * type, int * err);
void pmpi_file_read_all_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_read_at(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_read_at_all(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_read_at_all_begin(int* fh, long long * offset, void * buf, int * count, int * type, int * err);
void pmpi_file_read_at_all_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_read_ordered(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_read_ordered_begin(int* fh, void * buf, int * count, int * type, int * err);
void pmpi_file_read_ordered_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_read_shared(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_seek(int* fh, long long * offset, int * whence, int * err);
void pmpi_file_seek_shared(int* fh, long long * offset, int * whence, int * err);
void pmpi_file_set_view(int * fh, long long * offset, int * etype, int * filetype, char * datarep, int * info, int * err, int charlen);
void pmpi_file_sync(int* fh, int * err);
void pmpi_file_write(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_write_all(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_write_all_begin(int* fh, void * buf, int * count, int * type, int * err);
void pmpi_file_write_all_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_write_at(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_write_at_all(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_write_at_all_begin(int* fh, long long * offset, void * buf, int * count, int * type, int * err);
void pmpi_file_write_at_all_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_write_ordered(int* fh, void * buf, int * count, int * type, int * status, int * err);
void pmpi_file_write_ordered_begin(int* fh, void * buf, int * count, int * type, int * err);
void pmpi_file_write_ordered_end(int* fh, void * buf, int * status, int * err);
void pmpi_file_write_shared(int* fh, void * buf, int * count, int * type, int * status, int * err);

/*----------------------------------------------------------*/
/* Use weak symbols to cover one added underscore           */
/*----------------------------------------------------------*/
#ifdef ADD_UNDERSCORE
#pragma weak mpi_init__=mpi_init_
#pragma weak mpi_init_thread__=mpi_init_thread_
#pragma weak mpi_pcontrol__=mpi_pcontrol_
#pragma weak mpi_finalize__=mpi_finalize_
#pragma weak mpi_comm_rank__=mpi_comm_rank_
#pragma weak mpi_comm_size__=mpi_comm_size_
#pragma weak mpi_send__=mpi_send_
#pragma weak mpi_ssend__=mpi_ssend_
#pragma weak mpi_rsend__=mpi_rsend_
#pragma weak mpi_bsend__=mpi_bsend_
#pragma weak mpi_isend__=mpi_isend_
#pragma weak mpi_issend__=mpi_issend_
#pragma weak mpi_irsend__=mpi_irsend_
#pragma weak mpi_ibsend__=mpi_ibsend_
#pragma weak mpi_send_init__=mpi_send_init_
#pragma weak mpi_ssend_init__=mpi_ssend_init_
#pragma weak mpi_rsend_init__=mpi_rsend_init_
#pragma weak mpi_bsend_init__=mpi_bsend_init_
#pragma weak mpi_recv_init__=mpi_recv_init_
#pragma weak mpi_recv__=mpi_recv_
#pragma weak mpi_irecv__=mpi_irecv_
#pragma weak mpi_sendrecv__=mpi_sendrecv_
#pragma weak mpi_sendrecv_replace__=mpi_sendrecv_replace_
#pragma weak mpi_buffer_attach__=mpi_buffer_attach_
#pragma weak mpi_buffer_detach__=mpi_buffer_detach_
#pragma weak mpi_probe__=mpi_probe_
#pragma weak mpi_iprobe__=mpi_iprobe_
#pragma weak mpi_test__=mpi_test_
#pragma weak mpi_testany__=mpi_testany_
#pragma weak mpi_testall__=mpi_testall_
#pragma weak mpi_testsome__=mpi_testsome_
#pragma weak mpi_wait__=mpi_wait_
#pragma weak mpi_waitany__=mpi_waitany_
#pragma weak mpi_waitall__=mpi_waitall_
#pragma weak mpi_waitsome__=mpi_waitsome_
#pragma weak mpi_start__=mpi_start_
#pragma weak mpi_startall__=mpi_startall_
#pragma weak mpi_bcast__=mpi_bcast_
#pragma weak mpi_barrier__=mpi_barrier_
#pragma weak mpi_reduce__=mpi_reduce_
#pragma weak mpi_allreduce__=mpi_allreduce_
#pragma weak mpi_reduce_scatter__=mpi_reduce_scatter_
#pragma weak mpi_gather__=mpi_gather_
#pragma weak mpi_gatherv__=mpi_gatherv_
#pragma weak mpi_scan__=mpi_scan_
#pragma weak mpi_exscan__=mpi_exscan_
#pragma weak mpi_allgather__=mpi_allgather_
#pragma weak mpi_allgatherv__=mpi_allgatherv_
#pragma weak mpi_scatter__=mpi_scatter_
#pragma weak mpi_scatterv__=mpi_scatterv_
#pragma weak mpi_alltoall__=mpi_alltoall_
#pragma weak mpi_alltoallv__=mpi_alltoallv_
#pragma weak mpi_alltoallw__=mpi_alltoallw_

#if MPI_VERSION >= 3
#pragma weak mpi_ibcast__=mpi_ibcast_
#pragma weak mpi_ibarrier__=mpi_ibarrier_
#pragma weak mpi_ireduce__=mpi_ireduce_
#pragma weak mpi_iallreduce__=mpi_iallreduce_
#pragma weak mpi_ireduce_scatter__=mpi_ireduce_scatter_
#pragma weak mpi_ireduce_scatter_block__=mpi_ireduce_scatter_block_
#pragma weak mpi_igather__=mpi_igather_
#pragma weak mpi_igatherv__=mpi_igatherv_
#pragma weak mpi_iscan__=mpi_iscan_
#pragma weak mpi_iexscan__=mpi_iexscan_
#pragma weak mpi_iallgather__=mpi_iallgather_
#pragma weak mpi_neighbor_allgather__=mpi_neighbor_allgather_
#pragma weak mpi_ineighbor_allgather__=mpi_ineighbor_allgather_
#pragma weak mpi_iallgatherv__=mpi_iallgatherv_
#pragma weak mpi_neighbor_allgatherv__=mpi_neighbor_allgatherv_
#pragma weak mpi_ineighbor_allgatherv__=mpi_ineighbor_allgatherv_
#pragma weak mpi_iscatter__=mpi_iscatter_
#pragma weak mpi_iscatterv__=mpi_iscatterv_
#pragma weak mpi_ialltoall__=mpi_ialltoall_
#pragma weak mpi_neighbor_alltoall__=mpi_neighbor_alltoall_
#pragma weak mpi_ineighbor_alltoall__=mpi_ineighbor_alltoall_
#pragma weak mpi_ialltoallv__=mpi_ialltoallv_
#pragma weak mpi_neighbor_alltoallv__=mpi_neighbor_alltoallv_
#pragma weak mpi_ineighbor_alltoallv__=mpi_ineighbor_alltoallv_
#pragma weak mpi_ialltoallw__=mpi_ialltoallw_
#pragma weak mpi_neighbor_alltoallw__=mpi_neighbor_alltoallw_
#pragma weak mpi_ineighbor_alltoallw__=mpi_ineighbor_alltoallw_
#pragma weak mpi_accumulate__=mpi_accumulate_
#pragma weak mpi_fetch_and_op__=mpi_fetch_and_op_
#pragma weak mpi_get__=mpi_get_
#pragma weak mpi_get_accumulate__=mpi_get_accumulate_
#pragma weak mpi_put__=mpi_put_
#pragma weak mpi_raccumulate__=mpi_raccumulate_
#pragma weak mpi_reduce_scatter_block__=mpi_reduce_scatter_block_
#pragma weak mpi_rget__=mpi_rget_
#pragma weak mpi_rget_accumulate__=mpi_rget_accumulate_
#pragma weak mpi_rput__=mpi_rput_
#pragma weak mpi_win_allocate__=mpi_win_allocate_
#pragma weak mpi_win_allocate_shared__=mpi_win_allocate_shared_
#pragma weak mpi_win_attach__=mpi_win_attach_
#pragma weak mpi_win_complete__=mpi_win_complete_
#pragma weak mpi_win_create__=mpi_win_create_
#pragma weak mpi_win_create_dynamic__=mpi_win_create_dynamic_
#pragma weak mpi_win_detach__=mpi_win_detach_
#pragma weak mpi_win_fence__=mpi_win_fence_
#pragma weak mpi_win_flush__=mpi_win_flush_
#pragma weak mpi_win_flush_all__=mpi_win_flush_all_
#pragma weak mpi_win_flush_local__=mpi_win_flush_local_
#pragma weak mpi_win_flush_local_all__=mpi_win_flush_local_all_
#pragma weak mpi_win_free__=mpi_win_free_
#pragma weak mpi_win_lock__=mpi_win_lock_
#pragma weak mpi_win_lock_all__=mpi_win_lock_all_
#pragma weak mpi_win_post__=mpi_win_post_
#pragma weak mpi_win_start__=mpi_win_start_
#pragma weak mpi_win_sync__=mpi_win_sync_
#pragma weak mpi_win_test__=mpi_win_test_
#pragma weak mpi_win_unlock__=mpi_win_unlock_
#pragma weak mpi_win_unlock_all__=mpi_win_unlock_all_
#pragma weak mpi_win_wait__=mpi_win_wait_
#endif

#pragma weak mpi_file_close__=mpi_file_close_
#pragma weak mpi_file_delete__=mpi_file_delete_
#pragma weak mpi_file_iread__=mpi_file_iread_
#pragma weak mpi_file_iread_at__=mpi_file_iread_at_
#pragma weak mpi_file_iread_shared__=mpi_file_iread_shared_
#pragma weak mpi_file_iwrite__=mpi_file_iwrite_
#pragma weak mpi_file_iwrite_at__=mpi_file_iwrite_at_
#pragma weak mpi_file_iwrite_shared__=mpi_file_iwrite_shared_
#pragma weak mpi_file_open__=mpi_file_open_
#pragma weak mpi_file_preallocate__=mpi_file_preallocate_
#pragma weak mpi_file_read__=mpi_file_read_
#pragma weak mpi_file_read_all__=mpi_file_read_all_
#pragma weak mpi_file_read_all_begin__=mpi_file_read_all_begin_
#pragma weak mpi_file_read_all_end__=mpi_file_read_all_end_
#pragma weak mpi_file_read_at__=mpi_file_read_at_
#pragma weak mpi_file_read_at_all__=mpi_file_read_at_all_
#pragma weak mpi_file_read_at_all_begin__=mpi_file_read_at_all_begin_
#pragma weak mpi_file_read_at_all_end__=mpi_file_read_at_all_end_
#pragma weak mpi_file_read_ordered__=mpi_file_read_ordered_
#pragma weak mpi_file_read_ordered_begin__=mpi_file_read_ordered_begin_
#pragma weak mpi_file_read_ordered_end__=mpi_file_read_ordered_end_
#pragma weak mpi_file_read_shared__=mpi_file_read_shared_
#pragma weak mpi_file_seek__=mpi_file_seek_
#pragma weak mpi_file_seek_shared__=mpi_file_seek_shared_
#pragma weak mpi_file_set_view__=mpi_file_set_view_
#pragma weak mpi_file_sync__=mpi_file_sync_
#pragma weak mpi_file_write__=mpi_file_write_
#pragma weak mpi_file_write_all__=mpi_file_write_all_
#pragma weak mpi_file_write_all_begin__=mpi_file_write_all_begin_
#pragma weak mpi_file_write_all_end__=mpi_file_write_all_end_
#pragma weak mpi_file_write_at__=mpi_file_write_at_
#pragma weak mpi_file_write_at_all__=mpi_file_write_at_all_
#pragma weak mpi_file_write_at_all_begin__=mpi_file_write_at_all_begin_
#pragma weak mpi_file_write_at_all_end__=mpi_file_write_at_all_end_
#pragma weak mpi_file_write_ordered__=mpi_file_write_ordered_
#pragma weak mpi_file_write_ordered_begin__=mpi_file_write_ordered_begin_
#pragma weak mpi_file_write_ordered_end__=mpi_file_write_ordered_end_
#pragma weak mpi_file_write_shared__=mpi_file_write_shared_

#else

#pragma weak mpi_init_=mpi_init
#pragma weak mpi_init_thread_=mpi_init_thread
#pragma weak mpi_pcontrol_=mpi_pcontrol
#pragma weak mpi_finalize_=mpi_finalize
#pragma weak mpi_comm_rank_=mpi_comm_rank
#pragma weak mpi_comm_size_=mpi_comm_size
#pragma weak mpi_send_=mpi_send
#pragma weak mpi_ssend_=mpi_ssend
#pragma weak mpi_rsend_=mpi_rsend
#pragma weak mpi_bsend_=mpi_bsend
#pragma weak mpi_isend_=mpi_isend
#pragma weak mpi_issend_=mpi_issend
#pragma weak mpi_irsend_=mpi_irsend
#pragma weak mpi_ibsend_=mpi_ibsend
#pragma weak mpi_send_init_=mpi_send_init
#pragma weak mpi_ssend_init_=mpi_ssend_init
#pragma weak mpi_rsend_init_=mpi_rsend_init
#pragma weak mpi_bsend_init_=mpi_bsend_init
#pragma weak mpi_recv_init_=mpi_recv_init
#pragma weak mpi_recv_=mpi_recv
#pragma weak mpi_irecv_=mpi_irecv
#pragma weak mpi_sendrecv_=mpi_sendrecv
#pragma weak mpi_sendrecv_replace_=mpi_sendrecv_replace
#pragma weak mpi_buffer_attach_=mpi_buffer_attach
#pragma weak mpi_buffer_detach_=mpi_buffer_detach
#pragma weak mpi_probe_=mpi_probe
#pragma weak mpi_iprobe_=mpi_iprobe
#pragma weak mpi_test_=mpi_test
#pragma weak mpi_testany_=mpi_testany
#pragma weak mpi_testall_=mpi_testall
#pragma weak mpi_testsome_=mpi_testsome
#pragma weak mpi_wait_=mpi_wait
#pragma weak mpi_waitany_=mpi_waitany
#pragma weak mpi_waitall_=mpi_waitall
#pragma weak mpi_waitsome_=mpi_waitsome
#pragma weak mpi_start_=mpi_start
#pragma weak mpi_startall_=mpi_startall
#pragma weak mpi_bcast_=mpi_bcast
#pragma weak mpi_barrier_=mpi_barrier
#pragma weak mpi_reduce_=mpi_reduce
#pragma weak mpi_allreduce_=mpi_allreduce
#pragma weak mpi_reduce_scatter_=mpi_reduce_scatter
#pragma weak mpi_gather_=mpi_gather
#pragma weak mpi_gatherv_=mpi_gatherv
#pragma weak mpi_scan_=mpi_scan
#pragma weak mpi_exscan_=mpi_exscan
#pragma weak mpi_allgather_=mpi_allgather
#pragma weak mpi_allgatherv_=mpi_allgatherv
#pragma weak mpi_scatter_=mpi_scatter
#pragma weak mpi_scatterv_=mpi_scatterv
#pragma weak mpi_alltoall_=mpi_alltoall
#pragma weak mpi_alltoallv_=mpi_alltoallv
#pragma weak mpi_alltoallw_=mpi_alltoallw

#if MPI_VERSION >= 3
#pragma weak mpi_ibcast_=mpi_ibcast
#pragma weak mpi_ibarrier_=mpi_ibarrier
#pragma weak mpi_ireduce_=mpi_ireduce
#pragma weak mpi_iallreduce_=mpi_iallreduce
#pragma weak mpi_ireduce_scatter_=mpi_ireduce_scatter
#pragma weak mpi_ireduce_scatter_block_=mpi_ireduce_scatter_block
#pragma weak mpi_igather_=mpi_igather
#pragma weak mpi_igatherv_=mpi_igatherv
#pragma weak mpi_iscan_=mpi_iscan
#pragma weak mpi_iexscan_=mpi_iexscan
#pragma weak mpi_iallgather_=mpi_iallgather
#pragma weak mpi_neighbor_allgather_=mpi_neighbor_allgather
#pragma weak mpi_ineighbor_allgather_=mpi_ineighbor_allgather
#pragma weak mpi_iallgatherv_=mpi_iallgatherv
#pragma weak mpi_neighbor_allgatherv_=mpi_neighbor_allgatherv
#pragma weak mpi_ineighbor_allgatherv_=mpi_ineighbor_allgatherv
#pragma weak mpi_iscatter_=mpi_iscatter
#pragma weak mpi_iscatterv_=mpi_iscatterv
#pragma weak mpi_ialltoall_=mpi_ialltoall
#pragma weak mpi_neighbor_alltoall_=mpi_neighbor_alltoall
#pragma weak mpi_ineighbor_alltoall_=mpi_ineighbor_alltoall
#pragma weak mpi_ialltoallv_=mpi_ialltoallv
#pragma weak mpi_neighbor_alltoallv_=mpi_neighbor_alltoallv
#pragma weak mpi_ineighbor_alltoallv_=mpi_ineighbor_alltoallv
#pragma weak mpi_ialltoallw_=mpi_ialltoallw
#pragma weak mpi_neighbor_alltoallw_=mpi_neighbor_alltoallw
#pragma weak mpi_ineighbor_alltoallw_=mpi_ineighbor_alltoallw
#pragma weak mpi_accumulate_=mpi_accumulate
#pragma weak mpi_fetch_and_op_=mpi_fetch_and_op
#pragma weak mpi_get_=mpi_get
#pragma weak mpi_get_accumulate_=mpi_get_accumulate
#pragma weak mpi_put_=mpi_put
#pragma weak mpi_raccumulate_=mpi_raccumulate
#pragma weak mpi_reduce_scatter_block_=mpi_reduce_scatter_block
#pragma weak mpi_rget_=mpi_rget
#pragma weak mpi_rget_accumulate_=mpi_rget_accumulate
#pragma weak mpi_rput_=mpi_rput
#pragma weak mpi_win_allocate_=mpi_win_allocate
#pragma weak mpi_win_allocate_shared_=mpi_win_allocate_shared
#pragma weak mpi_win_attach_=mpi_win_attach
#pragma weak mpi_win_complete_=mpi_win_complete
#pragma weak mpi_win_create_=mpi_win_create
#pragma weak mpi_win_create_dynamic_=mpi_win_create_dynamic
#pragma weak mpi_win_detach_=mpi_win_detach
#pragma weak mpi_win_fence_=mpi_win_fence
#pragma weak mpi_win_flush_=mpi_win_flush
#pragma weak mpi_win_flush_all_=mpi_win_flush_all
#pragma weak mpi_win_flush_local_=mpi_win_flush_local
#pragma weak mpi_win_flush_local_all_=mpi_win_flush_local_all
#pragma weak mpi_win_free_=mpi_win_free
#pragma weak mpi_win_lock_=mpi_win_lock
#pragma weak mpi_win_lock_all_=mpi_win_lock_all
#pragma weak mpi_win_post_=mpi_win_post
#pragma weak mpi_win_start_=mpi_win_start
#pragma weak mpi_win_sync_=mpi_win_sync
#pragma weak mpi_win_test_=mpi_win_test
#pragma weak mpi_win_unlock_=mpi_win_unlock
#pragma weak mpi_win_unlock_all_=mpi_win_unlock_all
#pragma weak mpi_win_wait_=mpi_win_wait
#endif

#pragma weak mpi_file_close_=mpi_file_close
#pragma weak mpi_file_delete_=mpi_file_delete
#pragma weak mpi_file_iread_=mpi_file_iread
#pragma weak mpi_file_iread_at_=mpi_file_iread_at
#pragma weak mpi_file_iread_shared_=mpi_file_iread_shared
#pragma weak mpi_file_iwrite_=mpi_file_iwrite
#pragma weak mpi_file_iwrite_at_=mpi_file_iwrite_at
#pragma weak mpi_file_iwrite_shared_=mpi_file_iwrite_shared
#pragma weak mpi_file_open_=mpi_file_open
#pragma weak mpi_file_preallocate_=mpi_file_preallocate
#pragma weak mpi_file_read_=mpi_file_read
#pragma weak mpi_file_read_all_=mpi_file_read_all
#pragma weak mpi_file_read_all_begin_=mpi_file_read_all_begin
#pragma weak mpi_file_read_all_end_=mpi_file_read_all_end
#pragma weak mpi_file_read_at_=mpi_file_read_at
#pragma weak mpi_file_read_at_all_=mpi_file_read_at_all
#pragma weak mpi_file_read_at_all_begin_=mpi_file_read_at_all_begin
#pragma weak mpi_file_read_at_all_end_=mpi_file_read_at_all_end
#pragma weak mpi_file_read_ordered_=mpi_file_read_ordered
#pragma weak mpi_file_read_ordered_begin_=mpi_file_read_ordered_begin
#pragma weak mpi_file_read_ordered_end_=mpi_file_read_ordered_end
#pragma weak mpi_file_read_shared_=mpi_file_read_shared
#pragma weak mpi_file_seek_=mpi_file_seek
#pragma weak mpi_file_seek_shared_=mpi_file_seek_shared
#pragma weak mpi_file_set_view_=mpi_file_set_view
#pragma weak mpi_file_sync_=mpi_file_sync
#pragma weak mpi_file_write_=mpi_file_write
#pragma weak mpi_file_write_all_=mpi_file_write_all
#pragma weak mpi_file_write_all_begin_=mpi_file_write_all_begin
#pragma weak mpi_file_write_all_end_=mpi_file_write_all_end
#pragma weak mpi_file_write_at_=mpi_file_write_at
#pragma weak mpi_file_write_at_all_=mpi_file_write_at_all
#pragma weak mpi_file_write_at_all_begin_=mpi_file_write_at_all_begin
#pragma weak mpi_file_write_at_all_end_=mpi_file_write_at_all_end
#pragma weak mpi_file_write_ordered_=mpi_file_write_ordered
#pragma weak mpi_file_write_ordered_begin_=mpi_file_write_ordered_begin
#pragma weak mpi_file_write_ordered_end_=mpi_file_write_ordered_end
#pragma weak mpi_file_write_shared_=mpi_file_write_shared
#endif

/*----------------------------------------------------------*/
/*    Fortran-to-C interface with one underscore            */
/*----------------------------------------------------------*/
#ifdef ADD_UNDERSCORE
#define mpi_init                         mpi_init_ 
#define mpi_init_thread                  mpi_init_thread_ 
#define mpi_pcontrol                     mpi_pcontrol_ 
#define mpi_finalize                     mpi_finalize_ 
#define mpi_comm_rank                    mpi_comm_rank_ 
#define mpi_comm_size                    mpi_comm_size_ 
#define mpi_send                         mpi_send_ 
#define mpi_ssend                        mpi_ssend_ 
#define mpi_rsend                        mpi_rsend_ 
#define mpi_bsend                        mpi_bsend_ 
#define mpi_isend                        mpi_isend_ 
#define mpi_issend                       mpi_issend_ 
#define mpi_irsend                       mpi_irsend_ 
#define mpi_ibsend                       mpi_ibsend_ 
#define mpi_send_init                    mpi_send_init_ 
#define mpi_ssend_init                   mpi_ssend_init_ 
#define mpi_rsend_init                   mpi_rsend_init_ 
#define mpi_bsend_init                   mpi_bsend_init_ 
#define mpi_recv_init                    mpi_recv_init_ 
#define mpi_recv                         mpi_recv_ 
#define mpi_irecv                        mpi_irecv_ 
#define mpi_sendrecv                     mpi_sendrecv_ 
#define mpi_sendrecv_replace             mpi_sendrecv_replace_ 
#define mpi_buffer_attach                mpi_buffer_attach_ 
#define mpi_buffer_detach                mpi_buffer_detach_ 
#define mpi_probe                        mpi_probe_ 
#define mpi_iprobe                       mpi_iprobe_ 
#define mpi_test                         mpi_test_ 
#define mpi_testany                      mpi_testany_ 
#define mpi_testall                      mpi_testall_ 
#define mpi_testsome                     mpi_testsome_ 
#define mpi_wait                         mpi_wait_ 
#define mpi_waitany                      mpi_waitany_ 
#define mpi_waitall                      mpi_waitall_ 
#define mpi_waitsome                     mpi_waitsome_ 
#define mpi_start                        mpi_start_ 
#define mpi_startall                     mpi_startall_ 
#define mpi_bcast                        mpi_bcast_ 
#define mpi_barrier                      mpi_barrier_ 
#define mpi_reduce                       mpi_reduce_ 
#define mpi_allreduce                    mpi_allreduce_ 
#define mpi_reduce_scatter               mpi_reduce_scatter_ 
#define mpi_gather                       mpi_gather_ 
#define mpi_gatherv                      mpi_gatherv_ 
#define mpi_scan                         mpi_scan_ 
#define mpi_exscan                       mpi_exscan_ 
#define mpi_allgather                    mpi_allgather_ 
#define mpi_allgatherv                   mpi_allgatherv_ 
#define mpi_scatter                      mpi_scatter_ 
#define mpi_scatterv                     mpi_scatterv_ 
#define mpi_alltoall                     mpi_alltoall_ 
#define mpi_alltoallv                    mpi_alltoallv_ 
#define mpi_alltoallw                    mpi_alltoallw_ 

#if MPI_VERSION >= 3
#define mpi_ibcast                       mpi_ibcast_ 
#define mpi_ibarrier                     mpi_ibarrier_ 
#define mpi_ireduce                      mpi_ireduce_ 
#define mpi_iallreduce                   mpi_iallreduce_ 
#define mpi_ireduce_scatter              mpi_ireduce_scatter_ 
#define mpi_ireduce_scatter_block        mpi_ireduce_scatter_block_ 
#define mpi_igather                      mpi_igather_ 
#define mpi_igatherv                     mpi_igatherv_ 
#define mpi_iscan                        mpi_iscan_ 
#define mpi_iexscan                      mpi_iexscan_ 
#define mpi_iallgather                   mpi_iallgather_ 
#define mpi_neighbor_allgather           mpi_neighbor_allgather_ 
#define mpi_ineighbor_allgather          mpi_ineighbor_allgather_ 
#define mpi_iallgatherv                  mpi_iallgatherv_ 
#define mpi_neighbor_allgatherv          mpi_neighbor_allgatherv_ 
#define mpi_ineighbor_allgatherv         mpi_ineighbor_allgatherv_ 
#define mpi_iscatter                     mpi_iscatter_ 
#define mpi_iscatterv                    mpi_iscatterv_ 
#define mpi_ialltoall                    mpi_ialltoall_ 
#define mpi_neighbor_alltoall            mpi_neighbor_alltoall_ 
#define mpi_ineighbor_alltoall           mpi_ineighbor_alltoall_ 
#define mpi_ialltoallv                   mpi_ialltoallv_ 
#define mpi_neighbor_alltoallv           mpi_neighbor_alltoallv_ 
#define mpi_ineighbor_alltoallv          mpi_ineighbor_alltoallv_ 
#define mpi_ialltoallw                   mpi_ialltoallw_ 
#define mpi_neighbor_alltoallw           mpi_neighbor_alltoallw_ 
#define mpi_ineighbor_alltoallw          mpi_ineighbor_alltoallw_ 
#define mpi_accumulate                   mpi_accumulate_ 
#define mpi_fetch_and_op                 mpi_fetch_and_op_ 
#define mpi_get                          mpi_get_ 
#define mpi_get_accumulate               mpi_get_accumulate_ 
#define mpi_put                          mpi_put_ 
#define mpi_raccumulate                  mpi_raccumulate_ 
#define mpi_reduce_scatter_block         mpi_reduce_scatter_block_ 
#define mpi_rget                         mpi_rget_ 
#define mpi_rget_accumulate              mpi_rget_accumulate_ 
#define mpi_rput                         mpi_rput_ 
#define mpi_win_allocate                 mpi_win_allocate_ 
#define mpi_win_allocate_shared          mpi_win_allocate_shared_ 
#define mpi_win_attach                   mpi_win_attach_ 
#define mpi_win_complete                 mpi_win_complete_ 
#define mpi_win_create                   mpi_win_create_ 
#define mpi_win_create_dynamic           mpi_win_create_dynamic_ 
#define mpi_win_detach                   mpi_win_detach_ 
#define mpi_win_fence                    mpi_win_fence_ 
#define mpi_win_flush                    mpi_win_flush_ 
#define mpi_win_flush_all                mpi_win_flush_all_ 
#define mpi_win_flush_local              mpi_win_flush_local_ 
#define mpi_win_flush_local_all          mpi_win_flush_local_all_ 
#define mpi_win_free                     mpi_win_free_ 
#define mpi_win_lock                     mpi_win_lock_ 
#define mpi_win_lock_all                 mpi_win_lock_all_ 
#define mpi_win_post                     mpi_win_post_ 
#define mpi_win_start                    mpi_win_start_ 
#define mpi_win_sync                     mpi_win_sync_ 
#define mpi_win_test                     mpi_win_test_ 
#define mpi_win_unlock                   mpi_win_unlock_ 
#define mpi_win_unlock_all               mpi_win_unlock_all_ 
#define mpi_win_wait                     mpi_win_wait_ 
#endif

#define mpi_file_close                   mpi_file_close_ 
#define mpi_file_delete                  mpi_file_delete_ 
#define mpi_file_iread                   mpi_file_iread_ 
#define mpi_file_iread_at                mpi_file_iread_at_ 
#define mpi_file_iread_shared            mpi_file_iread_shared_ 
#define mpi_file_iwrite                  mpi_file_iwrite_ 
#define mpi_file_iwrite_at               mpi_file_iwrite_at_ 
#define mpi_file_iwrite_shared           mpi_file_iwrite_shared_ 
#define mpi_file_open                    mpi_file_open_ 
#define mpi_file_preallocate             mpi_file_preallocate_ 
#define mpi_file_read                    mpi_file_read_ 
#define mpi_file_read_all                mpi_file_read_all_ 
#define mpi_file_read_all_begin          mpi_file_read_all_begin_ 
#define mpi_file_read_all_end            mpi_file_read_all_end_ 
#define mpi_file_read_at                 mpi_file_read_at_ 
#define mpi_file_read_at_all             mpi_file_read_at_all_ 
#define mpi_file_read_at_all_begin       mpi_file_read_at_all_begin_ 
#define mpi_file_read_at_all_end         mpi_file_read_at_all_end_ 
#define mpi_file_read_ordered            mpi_file_read_ordered_ 
#define mpi_file_read_ordered_begin      mpi_file_read_ordered_begin_ 
#define mpi_file_read_ordered_end        mpi_file_read_ordered_end_ 
#define mpi_file_read_shared             mpi_file_read_shared_ 
#define mpi_file_seek                    mpi_file_seek_ 
#define mpi_file_seek_shared             mpi_file_seek_shared_ 
#define mpi_file_set_view                mpi_file_set_view_ 
#define mpi_file_sync                    mpi_file_sync_ 
#define mpi_file_write                   mpi_file_write_ 
#define mpi_file_write_all               mpi_file_write_all_ 
#define mpi_file_write_all_begin         mpi_file_write_all_begin_ 
#define mpi_file_write_all_end           mpi_file_write_all_end_ 
#define mpi_file_write_at                mpi_file_write_at_ 
#define mpi_file_write_at_all            mpi_file_write_at_all_ 
#define mpi_file_write_at_all_begin      mpi_file_write_at_all_begin_ 
#define mpi_file_write_at_all_end        mpi_file_write_at_all_end_ 
#define mpi_file_write_ordered           mpi_file_write_ordered_ 
#define mpi_file_write_ordered_begin     mpi_file_write_ordered_begin_ 
#define mpi_file_write_ordered_end       mpi_file_write_ordered_end_ 
#define mpi_file_write_shared            mpi_file_write_shared_ 
#endif


/*----------------------------------------------------------*/
/*    Fortran-to-C interface with no underscores            */
/*----------------------------------------------------------*/
#include "fortran_wrappers.c"
#endif
