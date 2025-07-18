#define NUM_COUNTERS 8
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
    "spr_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc0 cpu=0",
    "spr_unc_imc1::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc1 cpu=0",
    "spr_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc2 cpu=0",
    "spr_unc_imc3::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc3 cpu=0",
    "spr_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc4 cpu=0",
    "spr_unc_imc5::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc5 cpu=0",
    "spr_unc_imc6::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc6 cpu=0",
    "spr_unc_imc7::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc7 cpu=0"
  },  
// group 1:
  {
    "spr_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc0 cpu=1",
    "spr_unc_imc1::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc1 cpu=1",
    "spr_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc2 cpu=1",
    "spr_unc_imc3::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc3 cpu=1",
    "spr_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc4 cpu=1",
    "spr_unc_imc5::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc5 cpu=1",
    "spr_unc_imc6::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc6 cpu=1",
    "spr_unc_imc7::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc7 cpu=1"
  },  
// group 2:
  {
    "spr_unc_imc0::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc0 cpu=0",
    "spr_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc1 cpu=0",
    "spr_unc_imc2::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc2 cpu=0",
    "spr_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc3 cpu=0",
    "spr_unc_imc4::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc4 cpu=0",
    "spr_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc5 cpu=0",
    "spr_unc_imc6::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc6 cpu=0",
    "spr_unc_imc7::UNC_M_CAS_COUNT:WR:cpu=0", "writes to imc7 cpu=0" 
  },  
// group 3:
  {
    "spr_unc_imc0::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc0 cpu=1",
    "spr_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc1 cpu=1",
    "spr_unc_imc2::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc2 cpu=1",
    "spr_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc3 cpu=1",
    "spr_unc_imc4::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc4 cpu=1",
    "spr_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc5 cpu=1",
    "spr_unc_imc6::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc6 cpu=1",
    "spr_unc_imc7::UNC_M_CAS_COUNT:WR:cpu=1", "writes to imc7 cpu=1"
  },
// group 4:
  {
    "spr_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc0 cpu=0",
    "spr_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc1 cpu=0",
    "spr_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc2 cpu=0",
    "spr_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc3 cpu=0",
    "spr_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc4 cpu=0",
    "spr_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc5 cpu=0",
    "spr_unc_imc6::UNC_M_CAS_COUNT:RD:cpu=0", "reads from imc6 cpu=0",
    "spr_unc_imc7::UNC_M_CAS_COUNT:WR:cpu=0", "writes  to imc7 cpu=0"
  },
// group 5:
  {
    "spr_unc_imc0::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc0 cpu=1",
    "spr_unc_imc1::UNC_M_CAS_COUNT:WR:cpu=1", "writes  to imc1 cpu=1",
    "spr_unc_imc2::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc2 cpu=1",
    "spr_unc_imc3::UNC_M_CAS_COUNT:WR:cpu=1", "writes  to imc3 cpu=1",
    "spr_unc_imc4::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc4 cpu=1",
    "spr_unc_imc5::UNC_M_CAS_COUNT:WR:cpu=1", "writes  to imc5 cpu=1",
    "spr_unc_imc6::UNC_M_CAS_COUNT:RD:cpu=1", "reads from imc6 cpu=1",
    "spr_unc_imc7::UNC_M_CAS_COUNT:WR:cpu=1", "writes  to imc7 cpu=1"
  }
};
