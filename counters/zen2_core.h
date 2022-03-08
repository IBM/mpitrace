#define NUM_COUNTERS 4
#define MAX_GROUPS 3

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
     "perf::cycles",                "processor cycles",
     "perf::instructions",          "instructions retired",
     "LS_DISPATCH:LD_DISPATCH",     "load  uops dispatched to load/store unit",
     "RETIRED_SSE_AVX_FLOPS:ANY",   "weighted floating-point op count"
   },
// group 1:
   { 
     "perf::cycles",                "processor cycles",
     "perf::instructions",          "instructions retired",
     "LS_DISPATCH:LD_DISPATCH",     "load  uops dispatched to load/store unit",
     "LS_DISPATCH:STORE_DISPATCH",  "store uops dispatched to load/store unit"
   },
// group 2:
   { 
     "perf::cycles",                "processor cycles",
     "perf::instructions",          "instructions retired",
     "perf::branches",              "branch instructions retired",
     "perf::branch-misses",         "mispredicted branch instructions retired",
   }
};
