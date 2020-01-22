#define NUM_COUNTERS 6
#define MAX_GROUPS 22

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
  "PM_RUN_CYC", "Run cycles",
  "PM_ST_FIN", "Store instructions finished",
  "PM_LSU_FIN", "Load-store units finished an instruction",
  "PM_CMPLU_STALL", "Completion stall cycles",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_FLOP", "Floating Point instructions finished"
  },
  // group 1
  {
  "PM_CMPLU_STALL_LSU", "Completion stall caused by LSU instruction",
  "PM_CMPLU_STALL_SCALAR", "Completion stall caused by FPU instruction",
  "PM_TABLEWALK_CYC", "Cycles when a tablewalk (I or D) is active",
  "PM_DATA_TABLEWALK_CYC", "Cycles doing data tablewalks",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 2
  {
  "PM_LD_CMPL", "Loads completed",
  "PM_L3_ST_PREF", "L3 store Prefetches",
  "PM_L3_SW_PREF", "Data stream touch to L3",
  "PM_L3_PREF_ALL", "Total HW L3 prefetches",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 3
  {
  "PM_DATA_FROM_L2", "Data loaded from L2",
  "PM_DATA_FROM_L2MISS", "Demand load L2 miss",
  "PM_DATA_FROM_L3MISS", "demand load L3 miss",
  "PM_DATA_FROM_L3", "Data loaded from L3",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 4
  {
  "PM_DATA_FROM_L2MISS_MOD", "Demand loads that missed in the local L2",
  "PM_DATA_FROM_LMEM", "Data loaded from local memory",
  "PM_DATA_FROM_RMEM", "Data loaded from remote memory",
  "PM_DATA_FROM_DMEM", "Data loaded from distant memory",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 5
  {
  "PM_VSU0_1FLOP", "VSU0 one flop operations",
  "PM_VSU1_1FLOP", "VSU1 one flop operations",
  "PM_VSU0_2FLOP", "VSU0 two flop operations",
  "PM_VSU1_2FLOP", "VSU1 two flop operations",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 6
  {
  "PM_VSU0_4FLOP", "VSU0 four flop operations",
  "PM_VSU1_4FLOP", "VSU1 four flop operations",
  "PM_VSU0_8FLOP", "VSU0 eight flop operations",
  "PM_VSU1_8FLOP", "VSU1 eight flop operations",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 7
  {
  "PM_FLOP", "Floating-point instructions",
  "PM_FLOP_SUM_VEC", "Floating-point vector instructions",
  "PM_VSU0_STF", "Floating-point store from VSU0",
  "PM_VSU1_STF", "Floating-point store from VSU1",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 8
  {
  "PM_VSU0_FSQRT_FDIV", "VSU0 fdiv,fsqrt,xsdiv,xssqrt instructions",
  "PM_VSU1_FSQRT_FDIV", "VSU1 fdiv,fsqrt,xsdiv,xssqrt instructions",
  "PM_VSU1_16FLOP", "VSU1 16 flop operations",
  "PM_IPTEG_FROM_L3", "Page table entry loaded into TLB from local L3",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 9
  {
  "PM_LSU_FX_FIN", "Load-store units finished a FX operation",
  "PM_ST_FIN", "Store Instructions Finished",
  "PM_LSU_FIN", "Load-store units finished an instruction",
  "PM_LD_MISS_L1", "Load missed L1 D-cache",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 10
  {
  "PM_MEM_READ", "Reads from memory including L4 [shared core]",
  "PM_MEM_PREF", "Prefetches from memory [shared core]",
  "PM_LSU_FIN", "Load-store units finished an instruction",
  "PM_LD_MISS_L1", "Load missed L1 D-cache",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 11
  {
  "PM_FPU0_FCONV", "FPU0 executed FCONV instruction",
  "PM_FPU0_FEST", "FPU0 executed FEST instruction",
  "PM_FPU0_FRSP", "FPU0 executed FRSP instruction",
  "PM_LSU_LDF", "LSU executed Floating Point load instruction",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 12
  {
  "PM_FPU1_FCONV", "FPU1 executed FCONV instruction",
  "PM_FPU1_FEST", "FPU1 executed FEST instruction",
  "PM_FPU1_FRSP", "FPU1 executed FRSP instruction",
  "PM_LSU_LDX", "All Vector loads",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 13
  {
  "PM_VSU0_FIN", "VSU0 Finished an instruction",
  "PM_VSU1_FIN", "VSU1 Finished an instruction",
  "PM_FXU0_FIN", "Fixed point unit 0 finished an instruction",
  "PM_FXU1_FIN", "Fixed point unit 1 finished an instruction",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 14
  {
  "PM_BRU_FIN", "Branch Instruction Finished",
  "PM_LSU_FIN", "LSU Finished an instruction",
  "PM_ST_FIN", "Store Instructions Finished",
  "PM_CRU_FIN", "IFU Finished a non-branch instruction",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 15
  {
  "PM_L3_LD_PREF", "L3 Load Prefetches",
  "PM_CMPLU_STALL_VSU", "Completion stall due to VSU instruction",
  "PM_ST_MISS_L1", "Store Missed L1",
  "PM_CMPLU_STALL_VECTOR_LONG", "Completion stall due to VSU vector long instruction",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 16
  {
  "PM_CMPLU_STALL", "Completion stall cycles",
  "PM_CMPLU_STALL_FXU", "Completion stall caused by FXU instruction",
  "PM_CMPLU_STALL_FLUSH", "Completion stall due to flush by own thread",
  "PM_SHL_ST_DISABLE", "Store-Hit-Load Table Read Hit with entry Disabled",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 17
  {
  "PM_CMPLU_STALL_THRD", "Completion Stalled due to thread conflict",
  "PM_CMPLU_STALL_BRU_CRU", "Completion stall due to IFU",
  "PM_CMPLU_STALL_COQ_FULL", "Completion stall due to cast out queue full",
  "PM_CMPLU_STALL_BRU", "Completion stall due to a Branch Unit",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 18
  {
  "PM_DATA_ALL_PUMP_CPRED", "Pump prediction correct",
  "PM_CMPLU_STALL_NTCG_FLUSH", "Completion stall due to ntcg flush",
  "PM_L3_CO_MEPF", "L3 castouts in Mepf state [shared core]",
  "PM_CMPLU_STALL_LOAD_FINISH", "Completion stall due to a Load finish",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 19
  {
  "PM_1PLUS_PPC_CMPL", "One or more PPC instruction completed",
  "PM_NTCG_ALL_FIN", "Cycles after all instructions have finished to group completed",
  "PM_GRP_CMPL", "Group completed",
  "PM_CMPLU_STALL", "No groups completed, GCT not empty",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 20
  {
  "PM_GCT_NOSLOT_CYC", "Pipeline empty",
  "PM_CMPLU_STALL_LSU", "Completion stall caused by LSU instruction",
  "PM_FXU0_BUSY_FXU1_IDLE", "FXU0 busy FXU1 idle",
  "PM_CMPLU_STALL_FXLONG", "Completion stall due to a long latency fixed point instruction",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  },
  // group 21
  {
  "PM_L3_LD_PREF", "L3 Load Prefetches",
  "PM_L3_CO_MEM", "L3 cast outs to memory [shared core]",
  "PM_L3_PF_OFF_CHIP_CACHE", "L3 Prefetch from off chip cache [shared core]",
  "PM_L3_PF_OFF_CHIP_MEM", "L3 Prefetch from off chip memory [shared core]",
  "PM_RUN_INST_CMPL", "Run instructions completed",
  "PM_RUN_CYC", "Run cycles"
  }
};
