
// KNL core has 3 fixed counters and 2 programmable counters

#define NUM_COUNTERS 5
#define MAX_GROUPS 6

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
     "UOPS_RETIRED:SCALAR_SIMD",     "counts scalar SSE, AVX, AVX2, AVX-512 micro-ops except for loads, div, sqrt",
     "UOPS_RETIRED:PACKED_SIMD",     "counts packed SSE, AVX, AVX2, AVX-512 micro-ops except for loads, div, sqrt"
   },
 // group 1:
   {
     "perf::cycles",                 "processor cycles",
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "MEM_UOPS_RETIRED:ALL_LOADS",   "counts all the load micro-ops retired",
     "MEM_UOPS_RETIRED:ALL_STORES",  "counts all the store micro-ops retired"
   },
// group 2:
   { 
     "perf::cycles",                 "processor cycles",
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "perf::branch-instructions",    "branch instructions",
     "perf::branch-misses",          "branch instructions that were mis-predicted"
   },
// group 3:
   {
     "perf::cycles",                 "processor cycles",
     "perf::ref-cycles",             "cycles at the reference frequency",
     "perf::instructions",           "instructions completed",
     "perf::l1-icache-loads",        "all instruction fetches",
     "perf::l1-icache-load-misses",  "instruction fetches that missed the I-cache"
   }
};
