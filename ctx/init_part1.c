/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

   for (context=0; context<MAX_CONTEXTS; context++)
   {   
      memset(context_label[context], '\0', 80);
      synctime[context] = 0.0;
      context_starts[context] = 0L; 
      context_stops[context] = 0L; 

      for (id=0; id<MAX_IDS; id++)
      {   
         event_count[context][id] = 0LL;
         total_time[context][id]  = 0.0;
         total_bytes[context][id] = 0.0;

         for(bin=0; bin<MAX_BINS; bin++)
         {
            bin_count[context][id][bin] = 0LL;
            bin_bytes[context][id][bin] = 0.0;
            bin_time[context][id][bin]  = 0.0;
         }
      }   
   }

   strcpy(label[COMM_RANK_ID],                "MPI_Comm_rank");
   strcpy(label[COMM_SIZE_ID],                "MPI_Comm_size");
   strcpy(label[SEND_ID],                     "MPI_Send");
   strcpy(label[SSEND_ID],                    "MPI_Ssend");
   strcpy(label[RSEND_ID],                    "MPI_Rsend");
   strcpy(label[BSEND_ID],                    "MPI_Bsend");
   strcpy(label[ISEND_ID],                    "MPI_Isend");
   strcpy(label[ISSEND_ID],                   "MPI_Issend");
   strcpy(label[IRSEND_ID],                   "MPI_Irsend");
   strcpy(label[IBSEND_ID],                   "MPI_Ibsend");
   strcpy(label[SEND_INIT_ID],                "MPI_Send_init");
   strcpy(label[SSEND_INIT_ID],               "MPI_Ssend_init");
   strcpy(label[RSEND_INIT_ID],               "MPI_Rsend_init");
   strcpy(label[BSEND_INIT_ID],               "MPI_Bsend_init");
   strcpy(label[RECV_INIT_ID],                "MPI_Recv_init");
   strcpy(label[RECV_ID],                     "MPI_Recv");
   strcpy(label[IRECV_ID],                    "MPI_Irecv");
   strcpy(label[SENDRECV_ID],                 "MPI_Sendrecv");
   strcpy(label[SENDRECV_REPLACE_ID],         "MPI_Sendrecv_replace");
   strcpy(label[BUFFER_ATTACH_ID],            "MPI_Buffer_attach");
   strcpy(label[BUFFER_DETACH_ID],            "MPI_Buffer_detach");
   strcpy(label[PROBE_ID],                    "MPI_Probe");
   strcpy(label[IPROBE_ID],                   "MPI_Iprobe");
   strcpy(label[TEST_ID],                     "MPI_Test");
   strcpy(label[TESTANY_ID],                  "MPI_Testany");
   strcpy(label[TESTALL_ID],                  "MPI_Testall");
   strcpy(label[TESTSOME_ID],                 "MPI_Testsome");
   strcpy(label[WAIT_ID],                     "MPI_Wait");
   strcpy(label[WAITANY_ID],                  "MPI_Waitany");
   strcpy(label[WAITALL_ID],                  "MPI_Waitall");
   strcpy(label[WAITSOME_ID],                 "MPI_Waitsome");
   strcpy(label[START_ID],                    "MPI_Start");
   strcpy(label[STARTALL_ID],                 "MPI_Startall");
   strcpy(label[BCAST_ID],                    "MPI_Bcast");
   strcpy(label[IBCAST_ID],                   "MPI_Ibcast");
   strcpy(label[BARRIER_ID],                  "MPI_Barrier");
   strcpy(label[IBARRIER_ID],                 "MPI_Ibarrier");
   strcpy(label[REDUCE_ID],                   "MPI_Reduce");
   strcpy(label[IREDUCE_ID],                  "MPI_Ireduce");
   strcpy(label[ALLREDUCE_ID],                "MPI_Allreduce");
   strcpy(label[IALLREDUCE_ID],               "MPI_Iallreduce");
   strcpy(label[REDUCE_SCATTER_ID],           "MPI_Reduce_scatter");
   strcpy(label[IREDUCE_SCATTER_ID],          "MPI_Ireduce_scatter");
   strcpy(label[REDUCE_SCATTER_BLOCK_ID],     "MPI_Reduce_scatter_block");
   strcpy(label[IREDUCE_SCATTER_BLOCK_ID],    "MPI_Ireduce_scatter_block");
   strcpy(label[GATHER_ID],                   "MPI_Gather");
   strcpy(label[IGATHER_ID],                  "MPI_Igather");
   strcpy(label[GATHERV_ID],                  "MPI_Gatherv");
   strcpy(label[IGATHERV_ID],                 "MPI_Igatherv");
   strcpy(label[SCAN_ID],                     "MPI_Scan");
   strcpy(label[ISCAN_ID],                    "MPI_Iscan");
   strcpy(label[EXSCAN_ID],                   "MPI_Exscan");
   strcpy(label[IEXSCAN_ID],                  "MPI_Iexscan");
   strcpy(label[ALLGATHER_ID],                "MPI_Allgather");
   strcpy(label[NEIGHBOR_ALLGATHER_ID],       "MPI_Neighbor_allgather");
   strcpy(label[IALLGATHER_ID],               "MPI_Iallgather");
   strcpy(label[INEIGHBOR_ALLGATHER_ID],      "MPI_Ineighbor_allgather");
   strcpy(label[ALLGATHERV_ID],               "MPI_Allgatherv");
   strcpy(label[NEIGHBOR_ALLGATHERV_ID],      "MPI_Neighbor_allgatherv");
   strcpy(label[IALLGATHERV_ID],              "MPI_Iallgatherv");
   strcpy(label[INEIGHBOR_ALLGATHERV_ID],     "MPI_Ineighbor_allgatherv");
   strcpy(label[SCATTER_ID],                  "MPI_Scatter");
   strcpy(label[ISCATTER_ID],                 "MPI_Iscatter");
   strcpy(label[SCATTERV_ID],                 "MPI_Scatterv");
   strcpy(label[ISCATTERV_ID],                "MPI_Iscatterv");
   strcpy(label[ALLTOALL_ID],                 "MPI_Alltoall");
   strcpy(label[NEIGHBOR_ALLTOALL_ID],        "MPI_Neighbor_alltoall");
   strcpy(label[IALLTOALL_ID],                "MPI_Ialltoall");
   strcpy(label[INEIGHBOR_ALLTOALL_ID],       "MPI_Ineighbor_alltoall");
   strcpy(label[ALLTOALLV_ID],                "MPI_Alltoallv");
   strcpy(label[NEIGHBOR_ALLTOALLV_ID],       "MPI_Neighbor_alltoallv");
   strcpy(label[IALLTOALLV_ID],               "MPI_Ialltoallv");
   strcpy(label[INEIGHBOR_ALLTOALLV_ID],      "MPI_Ineighbor_alltoallv");
   strcpy(label[ALLTOALLW_ID],                "MPI_Alltoallw");
   strcpy(label[NEIGHBOR_ALLTOALLW_ID],       "MPI_Neighbor_alltoallw");
   strcpy(label[IALLTOALLW_ID],               "MPI_Ialltoallw");
   strcpy(label[INEIGHBOR_ALLTOALLW_ID],      "MPI_Ineighbor_alltoallw");
   strcpy(label[ACCUMULATE_ID],               "MPI_Accumulate");
   strcpy(label[FETCH_AND_OP_ID],             "MPI_Fetch_and_op");
   strcpy(label[GET_ID],                      "MPI_Get");
   strcpy(label[GET_ACCUMULATE_ID],           "MPI_Get_accumulate");
   strcpy(label[PUT_ID],                      "MPI_Put");
   strcpy(label[RACCUMULATE_ID],              "MPI_Raccumulate");
   strcpy(label[RGET_ID],                     "MPI_Rget");
   strcpy(label[RGET_ACCUMULATE_ID],          "MPI_Rget_accumulate");
   strcpy(label[RPUT_ID],                     "MPI_Rput");
   strcpy(label[WIN_ALLOCATE_ID],             "MPI_Win_allocate");
   strcpy(label[WIN_ALLOCATE_SHARED_ID],      "MPI_Win_allocate_shared");
   strcpy(label[WIN_ATTACH_ID],               "MPI_Win_attach");
   strcpy(label[WIN_COMPLETE_ID],             "MPI_Win_complete");
   strcpy(label[WIN_CREATE_ID],               "MPI_Win_create");
   strcpy(label[WIN_CREATE_DYNAMIC_ID],       "MPI_Win_create_dynamic");
   strcpy(label[WIN_DETACH_ID],               "MPI_Win_detach");
   strcpy(label[WIN_FENCE_ID],                "MPI_Win_fence");
   strcpy(label[WIN_FLUSH_ID],                "MPI_Win_flush");
   strcpy(label[WIN_FLUSH_ALL_ID],            "MPI_Win_flush_all");
   strcpy(label[WIN_FLUSH_LOCAL_ID],          "MPI_Win_flush_local");
   strcpy(label[WIN_FLUSH_LOCAL_ALL_ID],      "MPI_Win_flush_local_all");
   strcpy(label[WIN_FREE_ID],                 "MPI_Win_free");
   strcpy(label[WIN_LOCK_ID],                 "MPI_Win_lock");
   strcpy(label[WIN_LOCK_ALL_ID],             "MPI_Win_lock_all");
   strcpy(label[WIN_POST_ID],                 "MPI_Win_post");
   strcpy(label[WIN_START_ID],                "MPI_Win_start");
   strcpy(label[WIN_SYNC_ID],                 "MPI_Win_sync");
   strcpy(label[WIN_TEST_ID],                 "MPI_Win_test");
   strcpy(label[WIN_UNLOCK_ID],               "MPI_Win_unlock");
   strcpy(label[WIN_UNLOCK_ALL_ID],           "MPI_Win_unlock_all");
   strcpy(label[WIN_WAIT_ID],                 "MPI_Win_wait");

   strcpy(label[FILE_CLOSE_ID],               "MPI_File_close");
   strcpy(label[FILE_DELETE_ID],              "MPI_File_delete");
   strcpy(label[FILE_IREAD_ID],               "MPI_File_iread");
   strcpy(label[FILE_IREAD_AT_ID],            "MPI_File_iread_at");
   strcpy(label[FILE_IREAD_SHARED_ID],        "MPI_File_iread_shared");
   strcpy(label[FILE_IWRITE_ID],              "MPI_File_iwrite");
   strcpy(label[FILE_IWRITE_AT_ID],           "MPI_File_iwrite_at");
   strcpy(label[FILE_IWRITE_SHARED_ID],       "MPI_File_iwrite_shared");
   strcpy(label[FILE_OPEN_ID],                "MPI_File_open");
   strcpy(label[FILE_PREALLOCATE_ID],         "MPI_File_preallocate");
   strcpy(label[FILE_READ_ID],                "MPI_File_read");
   strcpy(label[FILE_READ_ALL_ID],            "MPI_File_read_all");
   strcpy(label[FILE_READ_ALL_BEGIN_ID],      "MPI_File_read_all_begin");
   strcpy(label[FILE_READ_ALL_END_ID],        "MPI_File_read_all_end");
   strcpy(label[FILE_READ_AT_ID],             "MPI_File_read_at");
   strcpy(label[FILE_READ_AT_ALL_ID],         "MPI_File_read_at_all");
   strcpy(label[FILE_READ_AT_ALL_BEGIN_ID],   "MPI_File_read_at_all_begin");
   strcpy(label[FILE_READ_AT_ALL_END_ID],     "MPI_File_read_at_all_end");
   strcpy(label[FILE_READ_ORDERED_ID],        "MPI_File_read_ordered");
   strcpy(label[FILE_READ_ORDERED_BEGIN_ID],  "MPI_File_read_ordered_begin");
   strcpy(label[FILE_READ_ORDERED_END_ID],    "MPI_File_read_ordered_end");
   strcpy(label[FILE_READ_SHARED_ID],         "MPI_File_read_shared");
   strcpy(label[FILE_SEEK_ID],                "MPI_File_seek");
   strcpy(label[FILE_SEEK_SHARED_ID],         "MPI_File_seek_shared");
   strcpy(label[FILE_SET_VIEW_ID],            "MPI_File_set_view");
   strcpy(label[FILE_SYNC_ID],                "MPI_File_sync");
   strcpy(label[FILE_WRITE_ID],               "MPI_File_write");
   strcpy(label[FILE_WRITE_ALL_ID],           "MPI_File_write_all");
   strcpy(label[FILE_WRITE_ALL_BEGIN_ID],     "MPI_File_write_all_begin");
   strcpy(label[FILE_WRITE_ALL_END_ID],       "MPI_File_write_all_end");
   strcpy(label[FILE_WRITE_AT_ID],            "MPI_File_write_at");
   strcpy(label[FILE_WRITE_AT_ALL_ID],        "MPI_File_write_at_all");
   strcpy(label[FILE_WRITE_AT_ALL_BEGIN_ID],  "MPI_File_write_at_all_begin");
   strcpy(label[FILE_WRITE_AT_ALL_END_ID],    "MPI_File_write_at_all_end");
   strcpy(label[FILE_WRITE_ORDERED_ID],       "MPI_File_write_ordered");
   strcpy(label[FILE_WRITE_ORDERED_BEGIN_ID], "MPI_File_write_ordered_begin");
   strcpy(label[FILE_WRITE_ORDERED_END_ID],   "MPI_File_write_ordered_end");
   strcpy(label[FILE_WRITE_SHARED_ID],        "MPI_File_write_shared");
