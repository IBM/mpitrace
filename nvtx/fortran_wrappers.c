/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

/*----------------------------------------------------------*/
/*    fortran entry points                                  */
/*----------------------------------------------------------*/
void mpi_init(int *);
#ifdef HAS_INIT_THREAD
void mpi_init_thread(int *, int *, int *);
#endif
void mpi_pcontrol(int * level);
void mpi_finalize(int *);
void mpi_abort(int *, int *, int *);
void mpi_comm_rank(int *, int *, int *);
void mpi_comm_size(int *, int *, int *);
void mpi_send(void *, int *, int *, int *, int *, int *, int *);
void mpi_ssend(void *, int *, int *, int *, int *, int *, int *);
void mpi_rsend(void *, int *, int *, int *, int *, int *, int *);
void mpi_bsend(void *, int *, int *, int *, int *, int *, int *);
void mpi_isend(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_issend(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_irsend(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_ibsend(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_send_init(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_ssend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_rsend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_bsend_init(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_recv_init(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_recv(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_irecv(void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_sendrecv(void *, int *, int *, int *, int *, void *, int *, int *, int *, int *, int *, MPI_Status *, int *);
void mpi_sendrecv_replace(void *, int *, int *, int *, int *, int *, int *, int *, MPI_Status *, int *);
void mpi_buffer_attach(void *, int *, int *);
void mpi_buffer_detach(void *, int *, int *);
void mpi_probe(int *, int *, int *, MPI_Status *, int *);
void mpi_iprobe(int *, int *, int *, int *, MPI_Status *, int *);
void mpi_test(int *, int *, MPI_Status *, int *);
void mpi_testany(int *, int *, int*, int *, MPI_Status *, int *);
void mpi_testall(int *, int*, int *, MPI_Status *, int *);
void mpi_testsome(int *, int *, int *, int *, MPI_Status *, int *);
void mpi_wait(int *, MPI_Status *, int *);
void mpi_waitany(int *, int *, int *, MPI_Status *, int *);
void mpi_waitall(int *, int *, MPI_Status *, int *);
void mpi_waitsome(int *, int *, int *, int *, MPI_Status *, int *);
void mpi_start(int *, int *);
void mpi_startall(int *, int *, int *);
void mpi_bcast(void *, int *, int *, int *, int *, int *);
void mpi_barrier(int *, int *);
void mpi_reduce(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_allreduce(void *, void *, int *, int *, int *, int *, int *);
void mpi_reduce_scatter(void *, void *, int *, int *, int *, int *, int *);
void mpi_gather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_gatherv(void *, int *, int *, void *, int *, int *, int *,int *, int *, int *);
void mpi_scan(void *, void *, int *, int *, int *, int *, int *);
void mpi_exscan(void *, void *, int *, int *, int *, int *, int *);
void mpi_allgather(void *, int *, int *, void *, int *, int *, int *, int *);
void mpi_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_scatter(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_scatterv(void *, int *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_alltoall(void *, int *, int *, void *, int *, int *, int *, int *);
void mpi_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void mpi_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);

#if MPI_VERSION >= 3
void mpi_ibcast(void *, int *, int *, int *, int *, int *, int *);
void mpi_ibarrier(int *, int *, int *);
void mpi_ireduce(void *, void *, int *, int *, int *, int *, int *, int *, int *);
void mpi_iallreduce(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_ireduce_scatter(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_ireduce_scatter_block(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_igather(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void mpi_igatherv(void *, int *, int *, void *, int *, int *, int *, int *,int *, int *, int *);
void mpi_iscan(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_iexscan(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_iallgather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_neighbor_allgather(void *, int *, int *, void *, int *, int *, int *, int *);
void mpi_ineighbor_allgather(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_iallgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void mpi_neighbor_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_ineighbor_allgatherv(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void mpi_iscatter(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void mpi_iscatterv(void *, int *, int *, int *, void *, int *, int *, int *, int *, int *, int *);
void mpi_ialltoall(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_neighbor_alltoall(void *, int *, int *, void *, int *, int *, int *, int *);
void mpi_ineighbor_alltoall(void *, int *, int *, void *, int *, int *, int *, int *, int *);
void mpi_ialltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void mpi_neighbor_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void mpi_ineighbor_alltoallv(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void mpi_ialltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void mpi_neighbor_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *);
void mpi_ineighbor_alltoallw(void *, int *, int *, int *, void *, int *, int *,int *, int *, int *, int *);
void mpi_accumulate(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_fetch_and_op(void *, void *, int *, int *, int *, int *, int *, int *);
void mpi_get(void *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_get_accumulate(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_put(void *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_raccumulate(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_reduce_scatter_block(void *, void *, int *, int *, int *, int *, int *);
void mpi_rget(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_rget_accumulate(void *, int *, int *, void *, int *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_rput(void *, int *, int *, int *, int *, int *, int *, int *, int *, int *);
void mpi_win_allocate(int *, int *, int *, int *, void *, int *, int *);
void mpi_win_allocate_shared(int *, int *, int *, int *, void *, int *, int *);
void mpi_win_attach(int *, void *, int *, int *);
void mpi_win_complete(int *, int *);
void mpi_win_create(void *, int *, int *, int *, int *, int *, int *);
void mpi_win_create_dynamic(int *, int *, int *, int *);
void mpi_win_detach(int *, void *, int *);
void mpi_win_fence(int *, int *, int *);
void mpi_win_flush(int *, int *, int *);
void mpi_win_flush_all(int *, int *);
void mpi_win_flush_local(int *, int *, int *);
void mpi_win_flush_local_all(int *, int *);
void mpi_win_free(int *, int *);
void mpi_win_lock(int *, int *, int *, int *, int *);
void mpi_win_lock_all(int *, int *, int *);
void mpi_win_post(int *, int *, int *, int *);
void mpi_win_start(int *, int *, int *, int *);
void mpi_win_sync(int *, int *);
void mpi_win_test(int*, int *, int *);
void mpi_win_unlock(int *, int *, int *);
void mpi_win_unlocl_all(int *, int *);
void mpi_win_wait(int *, int *);
#endif

/*-------------------------*/
/* Fortran MPI-IO Routines */
/*-------------------------*/
void mpi_file_close(int * fh, int * err);
void mpi_file_delete(char * filename, int * info, int * err, int charlen);
void mpi_file_iread(int * fh, void * buf, int * count, int * type, int * req, int * err);
void mpi_file_iread_at(int * fh, long long * offset, void * buf, int  * count, int * type, int * req, int * err);
void mpi_file_iread_shared(int * fh, void * buf, int * count, int * type, int * req, int * err);
void mpi_file_iwrite(int * fh, void * buf, int * count, int * type, int * req, int * err);
void mpi_file_iwrite_at(int * fh, long long * offset, void * buf, int * count, int * type, int * req, int * err);
void mpi_file_iwrite_shared(int * fh, void * buf, int * count, int * type, int * req, int * err);
void mpi_file_open(int * comm, char * filename, int * mode, int * info, int * fh, int * err, int charlen);
void mpi_file_preallocate(int * fh, long long * size_bytes, int * err);
void mpi_file_read(int * fh, void * buf, int  * count, int * type, int * status, int * err);
void mpi_file_read_all(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_read_all_begin(int* fh, void * buf, int * count, int * type, int * err);
void mpi_file_read_all_end(int* fh, void * buf, int * status, int * err);
void mpi_file_read_at(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_read_at_all(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_read_at_all_begin(int* fh, long long * offset, void * buf, int * count, int * type, int * err);
void mpi_file_read_at_all_end(int* fh, void * buf, int * status, int * err);
void mpi_file_read_ordered(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_read_ordered_begin(int* fh, void * buf, int * count, int * type, int * err);
void mpi_file_read_ordered_end(int* fh, void * buf, int * status, int * err);
void mpi_file_read_shared(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_seek(int* fh, long long * offset, int * whence, int * err);
void mpi_file_seek_shared(int* fh, long long * offset, int * whence, int * err);
void mpi_file_set_view(int * fh, long long * offset, int * etype, int * filetype, char * datarep, int * info, int * err, int charlen);
void mpi_file_sync(int* fh, int * err);
void mpi_file_write(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_write_all(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_write_all_begin(int* fh, void * buf, int * count, int * type, int * err);
void mpi_file_write_all_end(int* fh, void * buf, int * status, int * err);
void mpi_file_write_at(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_write_at_all(int* fh, long long * offset, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_write_at_all_begin(int* fh, long long * offset, void * buf, int * count, int * type, int * err);
void mpi_file_write_at_all_end(int* fh, void * buf, int * status, int * err);
void mpi_file_write_ordered(int* fh, void * buf, int * count, int * type, int * status, int * err);
void mpi_file_write_ordered_begin(int* fh, void * buf, int * count, int * type, int * err);
void mpi_file_write_ordered_end(int* fh, void * buf, int * status, int * err);
void mpi_file_write_shared(int* fh, void * buf, int * count, int * type, int * status, int * err);

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_init                         */
/*----------------------------------------------------------*/
void mpi_init(int * info)
{
   int i, id, rc, bin, argc, namelen;
   unsigned int limit;
   struct rusage RU;
   struct timeval TV;
   struct tm timeVal;
   time_t current_time;
   double frequency;
   char * ptr, ** argv;
   char * list_ptr;
   char delimiters[] = {","};
   char envstr[320];
   MPI_Errhandler err_traceback;

#include "init_part1.c"

   pmpi_init(&rc);

   *info = rc;

#include "init_part2.c"

   return;
}


/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_init_thread                  */
/*----------------------------------------------------------*/
#ifdef HAS_INIT_THREAD
void mpi_init_thread(int * required, int * provided, int * info)
{
   int i, id, rc, bin, argc, namelen;
   unsigned int limit;
   struct rusage RU;
   struct timeval TV;
   struct tm timeVal;
   time_t current_time;
   double frequency;
   char * ptr, ** argv;
   char * list_ptr;
   char delimiters[] = {","};
   char envstr[320];
   MPI_Errhandler err_traceback;

#include "init_part1.c"

   pmpi_init_thread(required, provided, &rc);

   *info = rc;

#include "init_part2.c"

   return;
}
#endif


/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_pcontrol                     */
/*----------------------------------------------------------*/
void mpi_pcontrol(int * level) 
{
    if (*level == 1)   mpitrace_summary_start();
    if (*level == 0)   mpitrace_summary_stop();
    if (*level == 101) mpitrace_trace_start();
    if (*level == 100) mpitrace_trace_stop();
    if (enable_tracing && (*level == 1)) mpitrace_trace_start();
    if (enable_tracing && (*level == 0)) mpitrace_trace_stop();
#ifdef VPROF
    if (*level == 11) mpitrace_vprof_start();
    if (*level == 10) mpitrace_vprof_stop();
#endif
#ifdef HPMPROF
    if (*level == 1)  HPM_Prof_start();
    if (*level == 0)  HPM_Prof_stop();
    if (*level == 21) HPM_Prof_start();
    if (*level == 20) HPM_Prof_stop();
#endif
#ifdef HPM
    if (*level == 1)  HPM_Start("pcontrol");
    if (*level == 0)  HPM_Stop("pcontrol");
#endif

    return;
}


/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_comm_rank                    */
/*----------------------------------------------------------*/
void mpi_comm_rank(int * comm, int * id, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[COMM_RANK_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_comm_rank(comm, id, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(COMM_RANK_ID, TV1, TV2, -1, -1, -1, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_comm_size                    */
/*----------------------------------------------------------*/
void mpi_comm_size(int * comm, int * ntasks, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[COMM_SIZE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_comm_size(comm, ntasks, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(COMM_SIZE_ID, TV1, TV2, -1, -1, -1, c_comm);
   return;
}


/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_send                         */
/*----------------------------------------------------------*/
void mpi_send(void * sbuf, int * count, int * type, int * dest, 
              int * tag, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_send(sbuf, count, type, dest, tag, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ssend                        */
/*----------------------------------------------------------*/
void mpi_ssend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ssend(sbuf, count, type, dest, tag, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_rsend                        */
/*----------------------------------------------------------*/
void mpi_rsend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_rsend(sbuf, count, type, dest, tag, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(RSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_bsend                        */
/*----------------------------------------------------------*/
void mpi_bsend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_bsend(sbuf, count, type, dest, tag, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(BSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_isend                        */
/*----------------------------------------------------------*/
void mpi_isend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_isend(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ISEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_issend                       */
/*----------------------------------------------------------*/
void mpi_issend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_issend(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ISSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_irsend                       */
/*----------------------------------------------------------*/
void mpi_irsend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IRSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_irsend(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IRSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ibsend                       */
/*----------------------------------------------------------*/
void mpi_ibsend(void * sbuf, int * count, int * type, int * dest, 
               int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBSEND_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ibsend(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IBSEND_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_send_init                    */
/*----------------------------------------------------------*/
void mpi_send_init(void * sbuf, int * count, int * type, int * dest, 
                   int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SEND_INIT_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_send_init(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SEND_INIT_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ssend_init                   */
/*----------------------------------------------------------*/
void mpi_ssend_init(void * sbuf, int * count, int * type, int * dest, 
                    int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SSEND_INIT_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ssend_init(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SSEND_INIT_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_rsend_init                   */
/*----------------------------------------------------------*/
void mpi_rsend_init(void * sbuf, int * count, int * type, int * dest, 
                    int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RSEND_INIT_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_rsend_init(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*dest == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(RSEND_INIT_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_bsend_init                   */
/*----------------------------------------------------------*/
void mpi_bsend_init(void * sbuf, int * count, int * type, int * dest, 
                    int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BSEND_INIT_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_bsend_init(sbuf, count, type, dest, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(BSEND_INIT_ID, TV1, TV2, -1, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_recv_init                    */
/*----------------------------------------------------------*/
void mpi_recv_init(void * rbuf, int * count, int * type, int * src, 
                   int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RECV_INIT_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_recv_init(rbuf, count, type, src, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*src == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(RECV_INIT_ID, TV1, TV2, *src, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_recv                         */
/*----------------------------------------------------------*/
void mpi_recv(void * rbuf, int * count, int * type, int * src, 
              int * tag, int * comm, int * status, int * info)
{
   int rc, bytes, size, count_received, source, flag;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RECV_ID]);
   MPI_Comm c_comm;
   MPI_Status local_status;

   c_comm = MPI_Comm_f2c(*comm);

   if (barrier_flag[RECV_ID])
   {
      flag = 0;
      WTIME(TV1);
      while (!flag)
      {
         WTIME(TV2);
         PMPI_Iprobe(*src, *tag, c_comm, &flag, &local_status);
      }
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_recv(rbuf, count, type, src, tag, comm, status, info);
   WTIME(TV2);

   if (status != MPI_F_STATUS_IGNORE)
   {
      source = ((MPI_Status *) status)->MPI_SOURCE;
      pmpi_get_count(status, type, &count_received, &rc);
   }
   else
   {
      count_received = *count;
      source = *src;
   }
   pmpi_type_size(type, &size, &rc);
   bytes = count_received * size;

   if (*src == MPI_PROC_NULL) bytes = 0;

   nvtxRangeEnd(range);
   LogEvent(RECV_ID, TV1, TV2, source, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_irecv                        */
/*----------------------------------------------------------*/
void mpi_irecv(void * rbuf, int * count, int * type, int * src, 
               int * tag, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IRECV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_irecv(rbuf, count, type, src, tag, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   if (*src == MPI_PROC_NULL) bytes = 0;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IRECV_ID, TV1, TV2, *src, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_sendrecv                     */
/*----------------------------------------------------------*/
void mpi_sendrecv(void * sbuf, int * scount, int * stype, int * dest, int * stag,
                  void * rbuf, int * rcount, int * rtype, int * src,  int * rtag,
                  int * comm, MPI_Status * status, int * info)
{
   int rc, sbytes, rbytes, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SENDRECV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_sendrecv(sbuf, scount, stype, dest,   stag,
                 rbuf, rcount, rtype, src, rtag,
                 comm, status, info);
   WTIME(TV2);

   pmpi_type_size(stype, &sbytes, &rc);
   sbytes = (*scount) * sbytes;

   pmpi_type_size(rtype, &rbytes, &rc);
   rbytes = (*rcount) * rbytes;

   bytes = 0;
   if (*dest != MPI_PROC_NULL) bytes += sbytes;
   if (*src  != MPI_PROC_NULL) bytes += rbytes;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SENDRECV_ID, TV1, TV2, *src, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_sendrecv_replace             */
/*----------------------------------------------------------*/
void mpi_sendrecv_replace(void * buf, int * count, int * type, int * dest, int * stag,
                          int * src, int * rtag, int * comm, MPI_Status * status, int * info)
{
   int rc, bytes, sbytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SENDRECV_REPLACE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_sendrecv_replace(buf, count, type, dest, stag,
                         src, rtag, comm, status, info);
   WTIME(TV2);

   pmpi_type_size(type, &sbytes, &rc);
   sbytes = (*count) * sbytes;

   bytes = 0;
   if (*dest != MPI_PROC_NULL) bytes += sbytes;
   if (*src  != MPI_PROC_NULL) bytes += sbytes;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SENDRECV_REPLACE_ID, TV1, TV2, *src, *dest, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_buffer_attach                */
/*----------------------------------------------------------*/
void mpi_buffer_attach(void * buffer, int * size, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BUFFER_ATTACH_ID]);

   WTIME(TV1);
   pmpi_buffer_attach(buffer, size, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(BUFFER_ATTACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_buffer_detach                */
/*----------------------------------------------------------*/
void mpi_buffer_detach(void * buffer, int * size, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BUFFER_DETACH_ID]);

   WTIME(TV1);
   pmpi_buffer_detach(buffer, size, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(BUFFER_DETACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_probe                        */
/*----------------------------------------------------------*/
void mpi_probe(int * src, int * tag, int * comm, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[PROBE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_probe(src, tag, comm, status, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(PROBE_ID, TV1, TV2, *src, -1, -1, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iprobe                       */
/*----------------------------------------------------------*/
void mpi_iprobe(int * src, int * tag, int * comm, int * flag, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IPROBE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iprobe(src, tag, comm, flag, status, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IPROBE_ID, TV1, TV2, *src, -1, -1, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_test                         */
/*----------------------------------------------------------*/
void mpi_test(int * request, int * flag, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TEST_ID]);

   WTIME(TV1);
   pmpi_test(request, flag, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TEST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_testany                      */
/*----------------------------------------------------------*/
void mpi_testany(int * num, int * req, int * indx, int * flag, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTANY_ID]);

   WTIME(TV1);
   pmpi_testany(num, req, indx, flag, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTANY_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_testall                      */
/*----------------------------------------------------------*/
void mpi_testall(int * num, int * req, int * flag, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTALL_ID]);

   WTIME(TV1);
   pmpi_testall(num, req, flag, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_testsome                     */
/*----------------------------------------------------------*/
void mpi_testsome(int * inum, int * req, int * onum, int * ind, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[TESTSOME_ID]);

   WTIME(TV1);
   pmpi_testsome(inum, req, onum, ind, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(TESTSOME_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_wait                         */
/*----------------------------------------------------------*/
void mpi_wait(int * request, MPI_Status * status, int * info)
{
   int src;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAIT_ID]);

   WTIME(TV1);
   pmpi_wait(request, status, info);
   WTIME(TV2);

   src = status->MPI_SOURCE;

   nvtxRangeEnd(range);
   LogEvent(WAIT_ID, TV1, TV2, src, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_waitany                      */
/*----------------------------------------------------------*/
void mpi_waitany(int * num, int * req, int * indx, MPI_Status * status, int * info)
{
   int src;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITANY_ID]);

   WTIME(TV1);
   pmpi_waitany(num, req, indx, status, info);
   WTIME(TV2);

   src = status->MPI_SOURCE;

   nvtxRangeEnd(range);
   LogEvent(WAITANY_ID, TV1, TV2, src, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_waitall                      */
/*----------------------------------------------------------*/
void mpi_waitall(int * num, int * req, MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITALL_ID]);

   WTIME(TV1);
   pmpi_waitall(num, req, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WAITALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_waitsome                     */
/*----------------------------------------------------------*/
void mpi_waitsome(int * inum, int * req, int * onum, int * ind, 
                  MPI_Status * status, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WAITSOME_ID]);

   WTIME(TV1);
   pmpi_waitsome(inum, req, onum, ind, status, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WAITSOME_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_start                        */
/*----------------------------------------------------------*/
void mpi_start(int * req, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[START_ID]);

   WTIME(TV1);
   pmpi_start(req, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(START_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_startall                     */
/*----------------------------------------------------------*/
void mpi_startall(int * num, int * req, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[STARTALL_ID]);

   WTIME(TV1);
   pmpi_startall(num, req, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(STARTALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_bcast                        */
/*----------------------------------------------------------*/
void mpi_bcast(void * data, int * count, int * type, 
               int * root, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BCAST_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[BCAST_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_bcast(data, count, type, root, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(BCAST_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ibcast                       */
/*----------------------------------------------------------*/
void mpi_ibcast(void * data, int * count, int * type, 
                int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBCAST_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ibcast(data, count, type, root, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IBCAST_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_barrier                      */
/*----------------------------------------------------------*/
void mpi_barrier(int * comm, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[BARRIER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_barrier(comm, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(BARRIER_ID, TV1, TV2, -1, -1, -1, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ibarrier                     */
/*----------------------------------------------------------*/
void mpi_ibarrier(int * comm, int * req, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IBARRIER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ibarrier(comm, req, info);
   WTIME(TV2);

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IBARRIER_ID, TV1, TV2, -1, -1, -1, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_reduce                       */
/*----------------------------------------------------------*/
void mpi_reduce(void * sbuf, void * rbuf, int * count, int * type, 
                int * op, int * root, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[REDUCE_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_reduce(sbuf, rbuf, count, type, op, root, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(REDUCE_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ireduce                      */
/*----------------------------------------------------------*/
void mpi_ireduce(void * sbuf, void * rbuf, int * count, int * type, 
                 int * op, int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ireduce(sbuf, rbuf, count, type, op, root, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IREDUCE_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_allreduce                    */
/*----------------------------------------------------------*/
void mpi_allreduce(void * sbuf, void * rbuf, int * count, int * type, 
                   int * op, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLREDUCE_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLREDUCE_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_allreduce(sbuf, rbuf, count, type, op, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLREDUCE_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iallreduce                   */
/*----------------------------------------------------------*/
void mpi_iallreduce(void * sbuf, void * rbuf, int * count, int * type, 
                   int * op, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLREDUCE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iallreduce(sbuf, rbuf, count, type, op, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLREDUCE_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_reduce_scatter               */
/*----------------------------------------------------------*/
void mpi_reduce_scatter(void * sbuf, void * rbuf, int * counts, int * type, 
                        int * op, int * comm, int * info)
{
   int i, rc, bytes, size, num, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_SCATTER_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[REDUCE_SCATTER_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_reduce_scatter(sbuf, rbuf, counts, type, op, comm, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(type, &size, &rc);

   num = 0;
   for (i=0; i<tasks; i++) num += counts[i];
   bytes = num * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(REDUCE_SCATTER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ireduce_scatter              */
/*----------------------------------------------------------*/
void mpi_ireduce_scatter(void * sbuf, void * rbuf, int * counts, int * type, 
                         int * op, int * comm, int * req, int * info)
{
   int i, rc, bytes, size, num, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_SCATTER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ireduce_scatter(sbuf, rbuf, counts, type, op, comm, req, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(type, &size, &rc);

   num = 0;
   for (i=0; i<tasks; i++) num += counts[i];
   bytes = num * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IREDUCE_SCATTER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_reduce_scatter_block         */
/*----------------------------------------------------------*/
void mpi_reduce_scatter_block(void * sbuf, void * rbuf, int * count, int * type, 
                              int * op, int * comm, int * info)
{
   int i, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[REDUCE_SCATTER_BLOCK_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[REDUCE_SCATTER_BLOCK_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_reduce_scatter_block(sbuf, rbuf, count, type, op, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);

   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(REDUCE_SCATTER_BLOCK_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ireduce_scatter_block        */
/*----------------------------------------------------------*/
void mpi_ireduce_scatter_block(void * sbuf, void * rbuf, int * count, int * type, 
                               int * op, int * comm, int * req, int * info)
{
   int i, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IREDUCE_SCATTER_BLOCK_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ireduce_scatter_block(sbuf, rbuf, count, type, op, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);

   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IREDUCE_SCATTER_BLOCK_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_gather                       */
/*----------------------------------------------------------*/
void mpi_gather(void * sbuf, int * scount, int * stype,
                void * rbuf, int * rcount, int * rtype,
                int * root, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GATHER_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[GATHER_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_gather(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(GATHER_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_igather                      */
/*----------------------------------------------------------*/
void mpi_igather(void * sbuf, int * scount, int * stype,
                 void * rbuf, int * rcount, int * rtype,
                 int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IGATHER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_igather(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IGATHER_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_gatherv                      */
/*----------------------------------------------------------*/
void mpi_gatherv(void * sbuf, int * scount, int * stype,
                 void * rbuf, int * rcounts, int * rdisp, int * rtype,
                 int * root, int * comm, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GATHERV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[GATHERV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_gatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, root, comm, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(GATHERV_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_igatherv                     */
/*----------------------------------------------------------*/
void mpi_igatherv(void * sbuf, int * scount, int * stype,
                  void * rbuf, int * rcounts, int * rdisp, int * rtype,
                  int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IGATHERV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_igatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, root, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IGATHERV_ID, TV1, TV2, -1, *root, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_scan                         */
/*----------------------------------------------------------*/
void mpi_scan(void * sbuf, void * rbuf, int * count, int * type, 
              int * op, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCAN_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[SCAN_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_scan(sbuf, rbuf, count, type, op, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SCAN_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iscan                        */
/*----------------------------------------------------------*/
void mpi_iscan(void * sbuf, void * rbuf, int * count, int * type, 
               int * op, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCAN_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iscan(sbuf, rbuf, count, type, op, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ISCAN_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_exscan                       */
/*----------------------------------------------------------*/
void mpi_exscan(void * sbuf, void * rbuf, int * count, int * type, 
                int * op, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[EXSCAN_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[EXSCAN_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_exscan(sbuf, rbuf, count, type, op, comm, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(EXSCAN_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iexscan                      */
/*----------------------------------------------------------*/
void mpi_iexscan(void * sbuf, void * rbuf, int * count, int * type, 
                 int * op, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IEXSCAN_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iexscan(sbuf, rbuf, count, type, op, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(type, &size, &rc);
   bytes = (*count) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IEXSCAN_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_allgather                    */
/*----------------------------------------------------------*/
void mpi_allgather(void * sbuf, int * scount, int * stype,
                   void * rbuf, int * rcount, int * rtype,
                   int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLGATHER_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLGATHER_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLGATHER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iallgather                   */
/*----------------------------------------------------------*/
void mpi_iallgather(void * sbuf, int * scount, int * stype,
                    void * rbuf, int * rcount, int * rtype,
                    int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLGATHER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iallgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLGATHER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_neighbor_allgather           */
/*----------------------------------------------------------*/
void mpi_neighbor_allgather(void * sbuf, int * scount, int * stype,
                            void * rbuf, int * rcount, int * rtype,
                            int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLGATHER_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[NEIGHBOR_ALLGATHER_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_neighbor_allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLGATHER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ineighbor_allgather          */
/*----------------------------------------------------------*/
void mpi_ineighbor_allgather(void * sbuf, int * scount, int * stype,
                             void * rbuf, int * rcount, int * rtype,
                             int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLGATHER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ineighbor_allgather(sbuf, scount, stype, rbuf, rcount, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(rtype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLGATHER_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_allgatherv                   */
/*----------------------------------------------------------*/
void mpi_allgatherv(void * sbuf, int * scount, int * stype,
                    void * rbuf, int * rcounts, int * rdisp, int * rtype,
                    int * comm, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLGATHERV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLGATHERV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(rtype, &size, &rc);
   bytes = rcounts[id] * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iallgatherv                  */
/*----------------------------------------------------------*/
void mpi_iallgatherv(void * sbuf, int * scount, int * stype,
                     void * rbuf, int * rcounts, int * rdisp, int * rtype,
                     int * comm, int * req, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLGATHERV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iallgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(rtype, &size, &rc);
   bytes = rcounts[id] * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLGATHERV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_neighbor_allgatherv          */
/*----------------------------------------------------------*/
void mpi_neighbor_allgatherv(void * sbuf, int * scount, int * stype,
                             void * rbuf, int * rcounts, int * rdisp, int * rtype,
                             int * comm, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLGATHERV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[NEIGHBOR_ALLGATHERV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_neighbor_allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(rtype, &size, &rc);
   bytes = rcounts[id] * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ineighbor_allgatherv         */
/*----------------------------------------------------------*/
void mpi_ineighbor_allgatherv(void * sbuf, int * scount, int * stype,
                              void * rbuf, int * rcounts, int * rdisp, int * rtype,
                              int * comm, int * req, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLGATHERV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ineighbor_allgatherv(sbuf, scount, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(rtype, &size, &rc);
   bytes = rcounts[id] * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLGATHERV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_scatter                      */
/*----------------------------------------------------------*/
void mpi_scatter(void * sbuf, int * scount, int * stype,
                 void * rbuf, int * rcount, int * rtype,
                 int * root, int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCATTER_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[SCATTER_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_scatter(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SCATTER_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iscatter                     */
/*----------------------------------------------------------*/
void mpi_iscatter(void * sbuf, int * scount, int * stype,
                  void * rbuf, int * rcount, int * rtype,
                  int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCATTER_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iscatter(sbuf, scount, stype, rbuf, rcount, rtype, root, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ISCATTER_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}
#endif 

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_scatterv                     */
/*----------------------------------------------------------*/
void mpi_scatterv(void * sbuf, int * scounts, int * sdisp, int * stype,
                  void * rbuf, int * rcount, int * rtype,
                  int * root, int * comm, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[SCATTERV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[SCATTERV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_scatterv(sbuf, scounts, sdisp, stype, rbuf, rcount, rtype, root, comm, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(stype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(SCATTERV_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_iscatterv                    */
/*----------------------------------------------------------*/
void mpi_iscatterv(void * sbuf, int * scounts, int * sdisp, int * stype,
                   void * rbuf, int * rcount, int * rtype,
                   int * root, int * comm, int * req, int * info)
{
   int rc, bytes, size, id;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ISCATTERV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_iscatterv(sbuf, scounts, sdisp, stype, rbuf, rcount, rtype, root, comm, req, info);
   WTIME(TV2);

   pmpi_comm_rank(comm, &id, &rc);
   pmpi_type_size(stype, &size, &rc);
   bytes = (*rcount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ISCATTERV_ID, TV1, TV2, *root, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_alltoall                     */
/*----------------------------------------------------------*/
void mpi_alltoall(void * sbuf, int * scount, int * stype,
                  void * rbuf, int * rcount, int * rtype,
                  int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALL_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLTOALL_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLTOALL_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ialltoall                    */
/*----------------------------------------------------------*/
void mpi_ialltoall(void * sbuf, int * scount, int * stype,
                   void * rbuf, int * rcount, int * rtype,
                   int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALL_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ialltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLTOALL_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_neighbor_alltoall            */
/*----------------------------------------------------------*/
void mpi_neighbor_alltoall(void * sbuf, int * scount, int * stype,
                           void * rbuf, int * rcount, int * rtype,
                           int * comm, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALL_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[NEIGHBOR_ALLTOALL_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_neighbor_alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALL_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ineighbor_alltoall           */
/*----------------------------------------------------------*/
void mpi_ineighbor_alltoall(void * sbuf, int * scount, int * stype,
                            void * rbuf, int * rcount, int * rtype,
                            int * comm, int * req, int * info)
{
   int rc, bytes, size;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALL_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ineighbor_alltoall(sbuf, scount, stype, rbuf, rcount, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_type_size(stype, &size, &rc);
   bytes = (*scount) * size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALL_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_alltoallv                    */
/*----------------------------------------------------------*/
void mpi_alltoallv(void * sbuf, int * scounts, int * sdisp, int * stype,
                   void * rbuf, int * rcounts, int * rdisp, int * rtype,
                   int * comm, int * info)
{
   int i, count, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALLV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLTOALLV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, 
                  rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(stype, &size, &rc);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * size) / tasks;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ialltoallv                   */
/*----------------------------------------------------------*/
void mpi_ialltoallv(void * sbuf, int * scounts, int * sdisp, int * stype,
                    void * rbuf, int * rcounts, int * rdisp, int * rtype,
                    int * comm, int * req, int * info)
{
   int i, count, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALLV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ialltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(stype, &size, &rc);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * size) / tasks;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLTOALLV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_neighbor_alltoallv           */
/*----------------------------------------------------------*/
void mpi_neighbor_alltoallv(void * sbuf, int * scounts, int * sdisp, int * stype,
                            void * rbuf, int * rcounts, int * rdisp, int * rtype,
                            int * comm, int * info)
{
   int i, count, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALLV_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[NEIGHBOR_ALLTOALLV_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_neighbor_alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(stype, &size, &rc);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * size) / tasks;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ineighbor_alltoallv          */
/*----------------------------------------------------------*/
void mpi_ineighbor_alltoallv(void * sbuf, int * scounts, int * sdisp, int * stype,
                             void * rbuf, int * rcounts, int * rdisp, int * rtype,
                             int * comm, int * req, int * info)
{
   int i, count, rc, bytes, size, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALLV_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ineighbor_alltoallv(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);
   pmpi_type_size(stype, &size, &rc);

   count = 0;
   for (i=0; i<tasks; i++) count += scounts[i];
   bytes = (count * size) / tasks;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALLV_ID, TV1, TV2, -1, -1, bytes, c_comm); 
   return;
}
#endif

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_alltoallw                    */
/*----------------------------------------------------------*/
void mpi_alltoallw(void * sbuf, int * scounts, int * sdisp, int * stype,
                   void * rbuf, int * rcounts, int * rdisp, int * rtype,
                   int * comm, int * info)
{
   int i, count, rc, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ALLTOALLW_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[ALLTOALLW_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp,
                  rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      pmpi_type_size(&stype[i], &sz, &rc);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ialltoallw                   */
/*----------------------------------------------------------*/
void mpi_ialltoallw(void * sbuf, int * scounts, int * sdisp, int * stype,
                    void * rbuf, int * rcounts, int * rdisp, int * rtype,
                    int * comm, int * req, int * info)
{
   int i, count, rc, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[IALLTOALLW_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ialltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      pmpi_type_size(&stype[i], &sz, &rc);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(IALLTOALLW_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_neighbor_alltoallw           */
/*----------------------------------------------------------*/
void mpi_neighbor_alltoallw(void * sbuf, int * scounts, int * sdisp, int * stype,
                            void * rbuf, int * rcounts, int * rdisp, int * rtype,
                            int * comm, int * info)
{
   int i, count, rc, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[NEIGHBOR_ALLTOALLW_ID]);
   MPI_Comm c_comm;

   if (barrier_flag[NEIGHBOR_ALLTOALLW_ID]) 
   {
      WTIME(TV1);
      pmpi_barrier(comm, &rc);
      WTIME(TV2);
      synctime += ( ( TCONV(TV2) ) - ( TCONV(TV1) ) );
   }

   WTIME(TV1);
   pmpi_neighbor_alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      pmpi_type_size(&stype[i], &sz, &rc);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(NEIGHBOR_ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_ineighbor_alltoallw          */
/*----------------------------------------------------------*/
void mpi_ineighbor_alltoallw(void * sbuf, int * scounts, int * sdisp, int * stype,
                             void * rbuf, int * rcounts, int * rdisp, int * rtype,
                             int * comm, int * req, int * info)
{
   int i, count, rc, bytes, sz, tasks;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[INEIGHBOR_ALLTOALLW_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_ineighbor_alltoallw(sbuf, scounts, sdisp, stype, rbuf, rcounts, rdisp, rtype, comm, req, info);
   WTIME(TV2);

   pmpi_comm_size(comm, &tasks, &rc);

   bytes = 0;
   for (i=0; i<tasks; i++)
   {
      pmpi_type_size(&stype[i], &sz, &rc);
      bytes += scounts[i]*sz;
   }

   bytes = (int) ( ((double) bytes) / ((double) tasks) );

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(INEIGHBOR_ALLTOALLW_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}
#endif

#if MPI_VERSION >= 3
/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_accumulate                   */
/*----------------------------------------------------------*/
void mpi_accumulate(void * origin_addr, int * origin_count, int * origin_datatype,
                    int * target_rank, int * target_disp, int * target_count, 
                    int * target_datatype, int * op, int * win, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[ACCUMULATE_ID]);

   WTIME(TV1);
   pmpi_accumulate(origin_addr, origin_count, origin_datatype, target_rank,
                   target_disp, target_count, target_datatype, op, win, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(ACCUMULATE_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_fetch_and_op                 */
/*----------------------------------------------------------*/
void mpi_fetch_and_op(void * origin_addr, void * result_addr, int * datatype,
                      int * target_rank, int * target_disp, int * op, int * win, int * info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FETCH_AND_OP_ID]);

   WTIME(TV1);
   pmpi_fetch_and_op(origin_addr, result_addr, datatype, target_rank, target_disp, op, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(FETCH_AND_OP_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_get                          */
/*----------------------------------------------------------*/
void mpi_get(void * origin_addr, int * origin_count, int * origin_datatype,
             int * target_rank, int * target_disp, int * target_count, 
             int * target_datatype, int * win, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GET_ID]);
 
   WTIME(TV1);
   pmpi_get(origin_addr, origin_count, origin_datatype, target_rank,
            target_disp, target_count, target_datatype, win, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(GET_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_get_accumulate               */
/*----------------------------------------------------------*/
void mpi_get_accumulate(void * origin_addr, int * origin_count, int * origin_datatype, 
                        void * result_addr, int * result_count, int * result_datatype, int * target_rank, 
                        int * target_disp, int * target_count, int * target_datatype, int *  op, int * win, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[GET_ACCUMULATE_ID]);

   WTIME(TV1);
   pmpi_get_accumulate(origin_addr, origin_count, origin_datatype, result_addr, result_count,
                       result_datatype, target_rank, target_disp, target_count, target_datatype, op, win, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(GET_ACCUMULATE_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_put                          */
/*----------------------------------------------------------*/
void mpi_put(void * origin_addr, int * origin_count, int * origin_datatype, 
             int * target_rank, int * target_disp, int * target_count, 
             int * target_datatype, int * win, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[PUT_ID]);

   WTIME(TV1);
   pmpi_put(origin_addr, origin_count, origin_datatype, target_rank, target_disp, 
            target_count, target_datatype, win, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(PUT_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_raccumulate                  */
/*----------------------------------------------------------*/
void mpi_raccumulate(void * origin_addr, int * origin_count, int * origin_datatype,
                     int * target_rank, int * target_disp, int * target_count, 
                     int * target_datatype, int * op, int * win, int * request, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RACCUMULATE_ID]);

   WTIME(TV1);
   pmpi_raccumulate(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
                    target_count, target_datatype, op, win, request, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(RACCUMULATE_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_rget                         */
/*----------------------------------------------------------*/
void mpi_rget(void * origin_addr, int * origin_count, int * origin_datatype,
              int * target_rank, int * target_disp, int * target_count, 
              int * target_datatype, int * win, int * request, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RGET_ID]);

   WTIME(TV1);
   pmpi_rget(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
             target_count, target_datatype, win, request, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(RGET_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_rget_accumulate              */
/*----------------------------------------------------------*/
void mpi_rget_accumulate(void *origin_addr, int *origin_count,
        int *origin_datatype, void *result_addr, int *result_count,
        int *result_datatype, int *target_rank, int *target_disp,
        int *target_count, int *target_datatype, int *op, int *win,
        int *request, int *info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RGET_ACCUMULATE_ID]);

   WTIME(TV1);
   pmpi_rget_accumulate(origin_addr, origin_count, origin_datatype, result_addr,
                        result_count, result_datatype, target_rank, target_disp,
                        target_count, target_datatype, op, win, request, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(RGET_ACCUMULATE_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_rput                         */
/*----------------------------------------------------------*/
void mpi_rput(void * origin_addr, int * origin_count, int * origin_datatype, 
              int * target_rank, int * target_disp, int * target_count, 
              int * target_datatype, int * win, int * request, int * info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[RPUT_ID]);

   WTIME(TV1);
   pmpi_rput(origin_addr, origin_count, origin_datatype, target_rank, target_disp,
             target_count, target_datatype, win, request, info);
   WTIME(TV2);

   pmpi_type_size(origin_datatype, &bytes, &rc);
   bytes = (*origin_count)*bytes;

   nvtxRangeEnd(range);
   LogEvent(RPUT_ID, TV1, TV2, -1, *target_rank, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_allocate                 */
/*----------------------------------------------------------*/
void mpi_win_allocate(int * size, int * disp_unit, int * info, 
                      int * comm, void * baseptr, int * win, int * ierr)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ALLOCATE_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_win_allocate(size, disp_unit, info, comm, baseptr, win, ierr);
   WTIME(TV2);

   bytes = *size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(WIN_ALLOCATE_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_allocate_shared          */
/*----------------------------------------------------------*/
void mpi_win_allocate_shared(int * size, int * disp_unit, int * info, 
                             int * comm, void * baseptr, int * win, int * ierr)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ALLOCATE_SHARED_ID]);
   MPI_Comm c_comm;

   WTIME(TV1);
   pmpi_win_allocate_shared(size, disp_unit, info, comm, baseptr, win, ierr);
   WTIME(TV2);

   bytes = *size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(WIN_ALLOCATE_SHARED_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_attach                   */
/*----------------------------------------------------------*/
void mpi_win_attach(int *win, void *base, int *size, int *info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_ATTACH_ID]);

   WTIME(TV1);
   pmpi_win_attach(win, base, size, info);
   WTIME(TV2);

   bytes = *size;

   nvtxRangeEnd(range);
   LogEvent(WIN_ATTACH_ID, TV1, TV2, -1, -1, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_complete                 */
/*----------------------------------------------------------*/
void mpi_win_complete(int *win, int *info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_COMPLETE_ID]);

   WTIME(TV1);
   pmpi_win_complete(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_COMPLETE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_create                   */
/*----------------------------------------------------------*/
void mpi_win_create(void * base, int * size, int * disp_unit, int * info, 
                    int * comm, int * win, int * ierr)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_CREATE_ID]);
   MPI_Comm c_comm;
 
   WTIME(TV1);
   pmpi_win_create(base, size, disp_unit, info, comm, win, ierr);
   WTIME(TV2);
 
   bytes = *size;

   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(WIN_CREATE_ID, TV1, TV2, -1, -1, bytes, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_create_dynamic           */
/*----------------------------------------------------------*/
void mpi_win_create_dynamic(int * info, int * comm, int * win, int * ierr)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_CREATE_DYNAMIC_ID]);
   MPI_Comm c_comm;
 
   WTIME(TV1);
   pmpi_win_create_dynamic(info, comm, win, ierr);
   WTIME(TV2);
 
   c_comm = MPI_Comm_f2c(*comm);
   nvtxRangeEnd(range);
   LogEvent(WIN_CREATE_DYNAMIC_ID, TV1, TV2, -1, -1, -1, c_comm);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_detach                   */
/*----------------------------------------------------------*/
void mpi_win_detach(int *win, void *base, int *info)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_DETACH_ID]);

   WTIME(TV1);
   pmpi_win_detach(win, base, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_DETACH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_fence                    */
/*----------------------------------------------------------*/
void mpi_win_fence(int * assert, int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FENCE_ID]);

   WTIME(TV1);
   pmpi_win_fence(assert, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FENCE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_flush                    */
/*----------------------------------------------------------*/
void mpi_win_flush(int * rank, int * win, int * info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_ID]);

   WTIME(TV1);
   pmpi_win_flush(rank, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_flush_all                */
/*----------------------------------------------------------*/
void mpi_win_flush_all(int * win, int * info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_ALL_ID]);

   WTIME(TV1);
   pmpi_win_flush_all(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_flush_local              */
/*----------------------------------------------------------*/
void mpi_win_flush_local(int * rank, int * win, int * info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_LOCAL_ID]);

   WTIME(TV1);
   pmpi_win_flush_local(rank, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_LOCAL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_flush_local_all          */
/*----------------------------------------------------------*/
void mpi_win_flush_local_all(int * win, int * info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FLUSH_LOCAL_ALL_ID]);

   WTIME(TV1);
   pmpi_win_flush_local_all(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FLUSH_LOCAL_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_free                     */
/*----------------------------------------------------------*/
void mpi_win_free(int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_FREE_ID]);

   WTIME(TV1);
   pmpi_win_free(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_FREE_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_lock                     */
/*----------------------------------------------------------*/
void mpi_win_lock(int * lock_type, int * rank, int * assert, int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_LOCK_ID]);

   WTIME(TV1);
   pmpi_win_lock(lock_type, rank, assert, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_LOCK_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_lock_all                 */
/*----------------------------------------------------------*/
void mpi_win_lock_all(int * assert, int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_LOCK_ALL_ID]);

   WTIME(TV1);
   pmpi_win_lock_all(assert, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_LOCK_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_post                     */
/*----------------------------------------------------------*/
void mpi_win_post(int *group, int *assert, int *win, int *info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_POST_ID]);

   WTIME(TV1);
   pmpi_win_post(group, assert, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_POST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_start                    */
/*----------------------------------------------------------*/
void mpi_win_start(int *group, int *assert, int *win, int *info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_START_ID]);

   WTIME(TV1);
   pmpi_win_start(group, assert, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_START_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_sync                     */
/*----------------------------------------------------------*/
void mpi_win_sync(int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_SYNC_ID]);
 
   WTIME(TV1);
   pmpi_win_sync(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_SYNC_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_test                     */
/*----------------------------------------------------------*/
void mpi_win_test(int *win, int *flag, int *info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_TEST_ID]);

   WTIME(TV1);
   pmpi_win_test(win, flag, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_TEST_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_unlock                   */
/*----------------------------------------------------------*/
void mpi_win_unlock(int * rank, int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_UNLOCK_ID]);

   WTIME(TV1);
   pmpi_win_unlock(rank, win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_UNLOCK_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_unlock_all               */
/*----------------------------------------------------------*/
void mpi_win_unlock_all(int * win, int * info)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_UNLOCK_ALL_ID]);

   WTIME(TV1);
   pmpi_win_unlock_all(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_UNLOCK_ALL_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_win_wait                     */
/*----------------------------------------------------------*/
void mpi_win_wait(int *win, int *info)
{
   int rc;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[WIN_WAIT_ID]);

   WTIME(TV1);
   pmpi_win_wait(win, info);
   WTIME(TV2);

   nvtxRangeEnd(range);
   LogEvent(WIN_WAIT_ID, TV1, TV2, -1, -1, -1, MPI_COMM_NULL);
   return;
}
#endif


/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_close                     */
/*----------------------------------------------------------*/
void mpi_file_close(int * fh, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_CLOSE_ID]);

   WTIME(TV1);
   pmpi_file_close(fh, err);
   WTIME(TV2);

   LogIOEvent(FILE_CLOSE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_delete                    */
/*----------------------------------------------------------*/
void mpi_file_delete(char * filename, int * info, int * err, int charlen)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_DELETE_ID]);

   WTIME(TV1);
   pmpi_file_delete(filename, info, err, charlen);
   WTIME(TV2);

   LogIOEvent(FILE_DELETE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iread                     */
/*----------------------------------------------------------*/
void mpi_file_iread(int * fh, void * buf, int * count, int * type, 
                    int * req, int * err)
{
   int  rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_ID]);

   WTIME(TV1);
   pmpi_file_iread(fh, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_IREAD_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iread_at                  */
/*----------------------------------------------------------*/
void mpi_file_iread_at(int * fh, long long * offset, void * buf, int  * count, 
                       int * type, int * req, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_AT_ID]);

   WTIME(TV1);
   pmpi_file_iread_at(fh, offset, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_IREAD_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iread_shared              */
/*----------------------------------------------------------*/
void mpi_file_iread_shared(int * fh, void * buf, int * count, 
                           int * type, int * req, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IREAD_SHARED_ID]);

   WTIME(TV1);
   pmpi_file_iread_shared(fh, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_IREAD_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iwrite                    */
/*----------------------------------------------------------*/
void mpi_file_iwrite(int * fh, void * buf, int * count, 
                     int * type, int * req, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_ID]);

   WTIME(TV1);
   pmpi_file_iwrite(fh, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_IWRITE_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iwrite_at                 */
/*----------------------------------------------------------*/
void mpi_file_iwrite_at(int * fh, long long * offset, void * buf, int * count, 
                        int * type, int * req, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_AT_ID]);

   WTIME(TV1);
   pmpi_file_iwrite_at(fh, offset, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_IWRITE_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_iwrite_shared             */
/*----------------------------------------------------------*/
void mpi_file_iwrite_shared(int * fh, void * buf, int * count, 
                            int * type, int * req, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_IWRITE_SHARED_ID]);

   WTIME(TV1);
   pmpi_file_iwrite_shared(fh, buf, count, type, req, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_IWRITE_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_open                      */
/*----------------------------------------------------------*/
void mpi_file_open(int * comm, char * filename, int * mode, 
                   int * info, int * fh, int * err, int charlen )
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_OPEN_ID]);

   WTIME(TV1);
   pmpi_file_open(comm, filename, mode, info, fh, err, charlen);
   WTIME(TV2);

   LogIOEvent(FILE_OPEN_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_preallocate               */
/*----------------------------------------------------------*/
void mpi_file_preallocate(int * fh, long long * size_bytes, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_PREALLOCATE_ID]);

   WTIME(TV1);
   pmpi_file_preallocate(fh, size_bytes, err);
   WTIME(TV2);

   LogIOEvent(FILE_PREALLOCATE_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read                      */
/*----------------------------------------------------------*/
void mpi_file_read(int * fh, void * buf, int  * count, 
                   int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ID]);

   WTIME(TV1);
   pmpi_file_read(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_all                  */
/*----------------------------------------------------------*/
void mpi_file_read_all(int* fh, void * buf, int * count, 
                       int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_ID]);

   WTIME(TV1);
   pmpi_file_read_all(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_ALL_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_all_begin            */
/*----------------------------------------------------------*/
void mpi_file_read_all_begin(int* fh, void * buf, int * count, 
                             int * type, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_read_all_begin(fh, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_ALL_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_all_end              */
/*----------------------------------------------------------*/
void mpi_file_read_all_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ALL_END_ID]);

   WTIME(TV1);
   pmpi_file_read_all_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_READ_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_at                   */
/*----------------------------------------------------------*/
void mpi_file_read_at(int* fh, long long * offset, void * buf, int * count, 
                      int * type, int * status, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ID]);

   WTIME(TV1);
   pmpi_file_read_at(fh, offset, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;
  
   loff = (long) (* offset);

   LogIOEvent(FILE_READ_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_at_all               */
/*----------------------------------------------------------*/
void mpi_file_read_at_all(int* fh, long long * offset, void * buf, int * count, 
                          int * type, int * status, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_ID]);

   WTIME(TV1);
   pmpi_file_read_at_all(fh, offset, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_READ_AT_ALL_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_at_all_begin         */
/*----------------------------------------------------------*/
void mpi_file_read_at_all_begin(int* fh, long long * offset, void * buf, 
                                int * count, int * type, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_read_at_all_begin(fh, offset, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_READ_AT_ALL_BEGIN_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_at_all_end           */
/*----------------------------------------------------------*/
void mpi_file_read_at_all_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_AT_ALL_END_ID]);

   WTIME(TV1);
   pmpi_file_read_at_all_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_READ_AT_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_ordered              */
/*----------------------------------------------------------*/
void mpi_file_read_ordered(int* fh, void * buf, int * count, 
                           int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_ID]);

   WTIME(TV1);
   pmpi_file_read_ordered(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_ORDERED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_ordered_begin        */
/*----------------------------------------------------------*/
void mpi_file_read_ordered_begin(int* fh, void * buf, int * count, 
                                 int * type, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_read_ordered_begin(fh, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_ORDERED_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_ordered_end          */
/*----------------------------------------------------------*/
void mpi_file_read_ordered_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_ORDERED_END_ID]);

   WTIME(TV1);
   pmpi_file_read_ordered_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_READ_ORDERED_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_read_shared               */
/*----------------------------------------------------------*/
void mpi_file_read_shared(int* fh, void * buf, int * count, 
                          int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_READ_SHARED_ID]);

   WTIME(TV1);
   pmpi_file_read_shared(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_READ_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_seek                      */
/*----------------------------------------------------------*/
void mpi_file_seek(int* fh, long long * offset, int * whence, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SEEK_ID]);
   long loff;

   WTIME(TV1);
   pmpi_file_seek(fh, offset, whence, err);
   WTIME(TV2);

   loff = (long) (* offset);

   LogIOEvent(FILE_SEEK_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_seek_shared               */
/*----------------------------------------------------------*/
void mpi_file_seek_shared(int* fh, long long * offset, int * whence, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SEEK_SHARED_ID]);
   long loff;

   WTIME(TV1);
   pmpi_file_seek_shared(fh, offset, whence, err);
   WTIME(TV2);

   loff = (long) (* offset);

   LogIOEvent(FILE_SEEK_SHARED_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_set_view                  */
/*----------------------------------------------------------*/
void mpi_file_set_view(int* fh, long long * offset, int * etype, int * filetype,
                       char * datarep, int * info, int * err, int charlen)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SET_VIEW_ID]);
   long loff;

   WTIME(TV1);
   pmpi_file_set_view(fh, offset, etype, filetype, datarep, info, err, charlen);
   WTIME(TV2);

   loff = (long) (* offset);

   LogIOEvent(FILE_SET_VIEW_ID, TV1, TV2, loff, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_sync                      */
/*----------------------------------------------------------*/
void mpi_file_sync(int* fh, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_SYNC_ID]);

   WTIME(TV1);
   pmpi_file_sync(fh, err);
   WTIME(TV2);

   LogIOEvent(FILE_SYNC_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write                     */
/*----------------------------------------------------------*/
void mpi_file_write(int* fh, void * buf, int * count, 
                    int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ID]);

   WTIME(TV1);
   pmpi_file_write(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_all                 */
/*----------------------------------------------------------*/
void mpi_file_write_all(int* fh, void * buf, int * count, 
                        int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_ID]);

   WTIME(TV1);
   pmpi_file_write_all(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_ALL_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_all_begin           */
/*----------------------------------------------------------*/
void mpi_file_write_all_begin(int* fh, void * buf, int * count, 
                              int * type, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_write_all_begin(fh, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_ALL_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_all_end             */
/*----------------------------------------------------------*/
void mpi_file_write_all_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ALL_END_ID]);

   WTIME(TV1);
   pmpi_file_write_all_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_at                  */
/*----------------------------------------------------------*/
void mpi_file_write_at(int* fh, long long * offset, void * buf, int * count, 
                       int * type, int * status, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ID]);

   WTIME(TV1);
   pmpi_file_write_at(fh, offset, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_WRITE_AT_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_at_all              */
/*----------------------------------------------------------*/
void mpi_file_write_at_all(int* fh, long long * offset, void * buf, int * count, 
                           int * type, int * status, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_ID]);

   WTIME(TV1);
   pmpi_file_write_at_all(fh, offset, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_WRITE_AT_ALL_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_at_all_begin        */
/*----------------------------------------------------------*/
void mpi_file_write_at_all_begin(int* fh, long long * offset, void * buf, 
                                 int * count, int * type, int * err)
{
   int rc, bytes;
   long loff;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_write_at_all_begin(fh, offset, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   loff = (long) (* offset);

   LogIOEvent(FILE_WRITE_AT_ALL_BEGIN_ID, TV1, TV2, loff, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_at_all_end          */
/*----------------------------------------------------------*/
void mpi_file_write_at_all_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_AT_ALL_END_ID]);

   WTIME(TV1);
   pmpi_file_write_at_all_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_AT_ALL_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_ordered             */
/*----------------------------------------------------------*/
void mpi_file_write_ordered(int* fh, void * buf, int * count, 
                            int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_ID]);

   WTIME(TV1);
   pmpi_file_write_ordered(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_ORDERED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_ordered_begin       */
/*----------------------------------------------------------*/
void mpi_file_write_ordered_begin(int* fh, void * buf, int * count, 
                                  int * type, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_BEGIN_ID]);

   WTIME(TV1);
   pmpi_file_write_ordered_begin(fh, buf, count, type, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_ORDERED_BEGIN_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_ordered_end         */
/*----------------------------------------------------------*/
void mpi_file_write_ordered_end(int* fh, void * buf, int * status, int * err)
{
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_ORDERED_END_ID]);

   WTIME(TV1);
   pmpi_file_write_ordered_end(fh, buf, status, err);
   WTIME(TV2);

   LogIOEvent(FILE_WRITE_ORDERED_END_ID, TV1, TV2, -1L, -1, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*  wrapper for Fortran: mpi_file_write_shared              */
/*----------------------------------------------------------*/
void mpi_file_write_shared(int* fh, void * buf, int * count, 
                           int * type, int * status, int * err)
{
   int rc, bytes;
   struct timeval TV1, TV2;
   nvtxRangeId_t range = nvtxRangeStartA(label[FILE_WRITE_SHARED_ID]);

   WTIME(TV1);
   pmpi_file_write_shared(fh, buf, count, type, status, err);
   WTIME(TV2);

   pmpi_type_size(type, &bytes, &rc);
   bytes = (*count) * bytes;

   LogIOEvent(FILE_WRITE_SHARED_ID, TV1, TV2, -1L, bytes, MPI_COMM_NULL);
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_finalize                     */
/*----------------------------------------------------------*/
void mpi_finalize(int * info)
{
   int rc;

   write_profile_data();

   rc = PMPI_Finalize();
  *info = rc;
   return;
}

/*----------------------------------------------------------*/
/*    wrapper for Fortran: mpi_abort                        */
/*----------------------------------------------------------*/
void mpi_abort(int * comm, int * err, int * info)
{
   if (traceback_abort) mpitrace_traceback(&taskid);
   pmpi_abort(comm, err, info);
   return;
}

