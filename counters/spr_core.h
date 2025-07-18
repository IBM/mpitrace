#define NUM_COUNTERS 6
#define MAX_GROUPS 5

struct eventStruct {
                      char * name;
                      char * label;
                   };

struct groupStruct {
                      struct eventStruct event[NUM_COUNTERS];
                   };


struct groupStruct CounterGroup[] = 
{
// group 0:
   { 
     "perf::cycles",                 "processor cycles",
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "perf::branch-instructions",    "branch instructions",
     "MEM_INST_RETIRED:ALL_LOADS",   "load instructions retired",
     "MEM_INST_RETIRED:ALL_STORES",  "store instructions retired"
   },
 // group 1:
   {
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "FP_ARITH_INST_RETIRED:SCALAR",  "scalar floating point instructions",
     "FP_ARITH_INST_RETIRED:VECTOR",  "vector floating point instructions",
     "FP_ARITH_INST_RETIRED:4_FLOPS", "simd instructions using four floats or four doubles",
     "FP_ARITH_INST_RETIRED:8_FLOPS", "simd instructions using eight floats or eight doubles"
   },
 // group 2:
   {
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "perf::l1-dcache-loads",        "load instructions",
     "perf::l1-dcache-stores",       "store instructions",
     "perf::l1-dcache-load-misses",  "loads that missed the L1 data cache",
     "ARITH:DIVIDER_ACTIVE",         "cycles doing division and square roots"
   },
 // group 3:
   {
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "CYCLE_ACTIVITY:STALLS_TOTAL",    "total execution stalls",
     "CYCLE_ACTIVITY:STALLS_L1D_MISS", "stalls waiting for L1 cache miss demand load",
     "CYCLE_ACTIVITY:STALLS_L2_MISS",  "stalls waiting for L2 cache miss demand load",
     "CYCLE_ACTIVITY:STALLS_L3_MISS",  "stalls waiting for L3 cache miss demand load",
   }
};
