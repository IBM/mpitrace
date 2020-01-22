#define NUM_COUNTERS 6
#define MAX_GROUPS 10

struct eventStruct {
                      char * name;
                      char * label;
                   };

struct groupStruct {
                      struct eventStruct event[NUM_COUNTERS];
                   };


struct groupStruct CounterGroup[] = 
{
  // group 0
  { 
     "PM_FLOP_CMPL",     "Floating Point instructions completed",
     "PM_ST_FIN",        "Store instructions finished",
     "PM_LSU_FIN",       "LSU instructions finished",
     "PM_CMPLU_STALL",   "Completion stall cycles",
     "PM_RUN_INST_CMPL", "Run instructions completed",
     "PM_RUN_CYC",       "Run cycles"
  },

  // group 1
  { 
     "PM_CMPLU_STALL_LSU",  "Completion stall by LSU instruction",
     "PM_CMPLU_STALL_THRD", "Completion stall due to thread conflict",
     "PM_CMPLU_STALL_BRU",  "Completion stall due to a Branch Unit",
     "PM_LD_MISS_L1",       "Load missed L1",
     "PM_RUN_INST_CMPL",    "Run instructions completed",
     "PM_RUN_CYC",          "Run cycles"
  },

  // group 2
  { 
     "PM_ICT_NOSLOT_CYC",        "Number of cycles the ICT has no itags assigned to this thread",
     "PM_BR_CMPL",               "Branch instructions completed",
     "PM_LSU_FIN",               "LSU instructions finished",
     "PM_CMPLU_STALL_EXEC_UNIT", "Completion stall due to execution units FXU/VSU/CRU",
     "PM_RUN_INST_CMPL",         "Run instructions completed",
     "PM_RUN_CYC",               "Run cycles"
  },

  // group 3
  { 
     "PM_INST_DISP",                "Instructions dispatched",
     "PM_CMPLU_STALL",              "Completion stall cycles",
     "PM_CMPLU_STALL_NTC_DISP_FIN", "Finish stall because the NTF instruction was one that must finish at dispatch",
     "PM_NTC_ALL_FIN",              "Cycles after all instructions have finished to group completed",
     "PM_RUN_INST_CMPL",            "Run instructions completed",
     "PM_RUN_CYC",                  "Run cycles"
  },

  // group 4
  { 
     "PM_NTC_FIN",                "Cycles in which the oldest instruction in the pipeline finishes",
     "PM_CMPLU_STALL_OTHER_CMPL", "Instructions the core completed while this thread was stalled",
     "PM_1FLOP_CMPL",             "VSU one-flop instructions completed",
     "PM_DATA_FROM_L2",           "The data cache was reloaded from the local L2 due to a demand load",
     "PM_RUN_INST_CMPL",          "Run instructions completed",
     "PM_RUN_CYC",                "Run cycles"
  },

  // group 5
  { 
     "PM_DATA_FROM_L2",     "The data cache was reloaded from the local L2 due to a demand load",
     "PM_DATA_FROM_L2MISS", "Demand load that missed in L2",
     "PM_DATA_FROM_L3MISS", "Demand load that missed in both L2 and L3",
     "PM_DATA_FROM_L3",     "The data cache was reloaded from L3 due to a demand load",
     "PM_RUN_INST_CMPL",    "Run instructions completed",
     "PM_RUN_CYC",          "Run cycles"
  },

  // group 6
  { 
     "PM_LSU_FIN",       "LSU instructions finished",
     "PM_VSU_FIN",       "VSU instructions finished",
     "PM_FXU_FIN",       "FXU instructions finished",
     "PM_BRU_FIN",       "Branch instructions finished",
     "PM_RUN_INST_CMPL", "Run instructions completed",
     "PM_RUN_CYC",       "Run cycles"
  },

  // group 7
  { 
     "PM_MEM_READ",      "Reads from memory from this thread",
     "PM_MEM_PREF",      "Prefetches from memory for this thread",
     "PM_MEM_RWITM",     "Reads from memory with intent to modify from this thread",
     "PM_2FLOP_CMPL",    "VSU two-flop instructions completed",
     "PM_RUN_INST_CMPL", "Run instructions completed",
     "PM_RUN_CYC",       "Run cycles"
  },

  // group 8
  { 
     "PM_4FLOP_CMPL",     "VSU four-flop instructions completed",
     "PM_L1_ICACHE_MISS", "L1 I-cache misses",
     "PM_LD_REF_L1",      "L1 D-cache load references counted at finish, gated by reject",
     "PM_LD_MISS_L1",     "Load missed L1",
     "PM_RUN_INST_CMPL",  "Run instructions completed",
     "PM_RUN_CYC",        "Run cycles"
  },

  // group 9
  { 
     "PM_8FLOP_CMPL",     "VSU eight-flop instructions completed",
     "PM_ST_CMPL",        "Stores completed",
     "PM_L3_LD_PREF",     "L3 load prefetch from a hardware or software stream",
     "PM_L3_SW_PREF",     "L3 load prefetch from a software stream",
     "PM_RUN_INST_CMPL",  "Run instructions completed",
     "PM_RUN_CYC",        "Run cycles"
  }

};
