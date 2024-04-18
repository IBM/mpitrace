#define NUM_COUNTERS 2
#define MAX_GROUPS 14

struct eventStruct {
                      char * name;
                      char * label;
                   };

struct groupStruct {
                      struct eventStruct event[NUM_COUNTERS];
                   };


struct groupStruct CounterGroup[] =
{
  { // Group 1
  "PAPI_L1_DCM",   "Level 1 data cache misses",
  "PAPI_L1_ICM",   "Level 1 instruction cache misses",
  },{ // Group 2
  "PAPI_L2_DCM",   "Level 2 data cache misses",
  "PAPI_TLB_DM",   "Data translation lookaside buffer misses",
  },{ // Group 3
  "PAPI_L2_LDM",   "Level 2 load misses",
  "PAPI_STL_ICY",  "Cycles with no instruction issue",
  },{ // Group 4
  "PAPI_HW_INT",  "Hardware interrupts",
  "PAPI_BR_MSP",   "Conditional branch instructions mispredicted",
  },{ // Group 5
  "PAPI_BR_PRC",  "Conditional branch instructions correctly predicted",
  "PAPI_TOT_INS",   "Instructions completed",
  },{ // Group 6
  "PAPI_FP_INS",   "Floating point instructions",
  "PAPI_LD_INS",   "Load instructions",
  },{ // Group 7
  "PAPI_SR_INS",   "Store instructions",
  "PAPI_BR_INS",   "Branch instructions",
  },{ // Group 8
  "PAPI_VEC_INS",  "Vector/SIMD instructions (could include integer)",
  "PAPI_RES_STL",   "Cycles stalled on any resource",
  },{ // Group 9
  "PAPI_TOT_CYC",   "Total cycles",
  "PAPI_LST_INS",  "Load/store instructions completed",
  },{ // Group 10
  "PAPI_SYC_INS",  "Synchronization instructions completed",
  "PAPI_L1_DCA",   "Level 1 data cache accesses",
  },{ // Group 11
  "PAPI_L2_DCA",  "Level 2 data cache accesses",
  "PAPI_L1_DCR",   "Level 1 data cache reads",
  },{ // Group 12
  "PAPI_L2_DCR",   "Level 2 data cache reads",
  "PAPI_L1_DCW",   "Level 1 data cache writes",
  },{ // Group 13
  "PAPI_L2_DCW",   "Level 2 data cache writes",
  "PAPI_L1_ICH",  "Level 1 instruction cache hits",
  },{ // Group 14
  "PAPI_L1_ICA",   "Level 1 instruction cache accesses",
  "PAPI_L2_TCA",   "Level 2 total cache accesses",
  }
};
