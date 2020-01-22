#define NUM_COUNTERS 5
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
     "CPU_CYCLES",     "processor cycles",
     "INST_RETIRED",   "instructions retired",
     "MEM_ACCESS_RD",  "load   instructions",
     "MEM_ACCESS_WR",  "store  instructions",
     "BR_RETIRED",     "branch instructions",
   }
};
