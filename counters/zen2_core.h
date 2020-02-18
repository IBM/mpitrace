#define NUM_COUNTERS 6
#define MAX_GROUPS 1

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
     "perf::branches",              "branch instructions retired",
     "perf::branch-misses",         "mispredicted branch instructions retired",
     "LS_DISPATCH:LD_DISPATCH",     "load  uops dispatched to load/store unit",
     "LS_DISPATCH:STORE_DISPATCH",  "store uops dispatched to load/store unit"
   }
};
