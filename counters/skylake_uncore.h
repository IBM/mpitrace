#define NUM_COUNTERS 6
#define MAX_GROUPS 4

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
    "skx_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc0 cpu=0",
    "skx_unc_imc1::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc1 cpu=0",
    "skx_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc2 cpu=0",
    "skx_unc_imc3::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc3 cpu=0",
    "skx_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc4 cpu=0",
    "skx_unc_imc5::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc5 cpu=0"
  },  
// group 1:
  {
    "skx_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc0 cpu=1",
    "skx_unc_imc1::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc1 cpu=1",
    "skx_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc2 cpu=1",
    "skx_unc_imc3::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc3 cpu=1",
    "skx_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc4 cpu=1",
    "skx_unc_imc5::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc5 cpu=1"
  },  
// group 2:
  {
    "skx_unc_imc0::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc0 cpu=0",
    "skx_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc1 cpu=0",
    "skx_unc_imc2::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc2 cpu=0",
    "skx_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc3 cpu=0",
    "skx_unc_imc4::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc4 cpu=0",
    "skx_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=0", "writes from imc5 cpu=0",
  },  
// group 3:
  {
    "skx_unc_imc0::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc0 cpu=1",
    "skx_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc1 cpu=1",
    "skx_unc_imc2::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc2 cpu=1",
    "skx_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc3 cpu=1",
    "skx_unc_imc4::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc4 cpu=1",
    "skx_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=1", "writes from imc5 cpu=1"
  }   
};
