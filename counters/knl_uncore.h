#define NUM_COUNTERS 6
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
    "knl_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc0 cpu=0",
    "knl_unc_imc1::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc1 cpu=0",
    "knl_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc2 cpu=0",
    "knl_unc_imc3::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc3 cpu=0",
    "knl_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc4 cpu=0",
    "knl_unc_imc5::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc5 cpu=0"
  },
// group 1
  {
    "knl_unc_imc0::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc0 cpu=0",
    "knl_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc1 cpu=0",
    "knl_unc_imc2::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc2 cpu=0",
    "knl_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc3 cpu=0",
    "knl_unc_imc4::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc4 cpu=0",
    "knl_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc5 cpu=0"
  },
// group 2
  {
    "knl_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc0 cpu=0",
    "knl_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc1 cpu=0",
    "knl_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc2 cpu=0",
    "knl_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc3 cpu=0",
    "knl_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc4 cpu=0",
    "knl_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc5 cpu=0"
  }
};
