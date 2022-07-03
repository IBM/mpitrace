#define _GNU_SOURCE
#include <mpi.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <execinfo.h> 
#include <sys/resource.h>
#include <utmpx.h>
#ifdef USE_NVTX
#include <cuda_profiler_api.h>
#include <nvToolsExt.h>
#endif
#ifdef USE_ROCTX
#include <roctracer_ext.h>
#include <roctx.h>
#endif
#include <dlfcn.h>
#include <stdint.h>

/*----------------------------------------------------------*/
/*    dimensions of arrays                                  */
/*----------------------------------------------------------*/
#define MAX_BINS 31
#define MAX_IDS 146

#define SORT_ASCENDING_ORDER   1
#define SORT_DESCENDING_ORDER -1

/*----------------------------------------------------------*/
/*    define a structure to hold trace events               */
/*----------------------------------------------------------*/
static int max_events = 100000;

struct intPair { int src; int dest; };

typedef union { long offset; struct intPair ranks; } unType;

struct eventstruct {
                     double tbeg;
                     double tend;
                     int taskid;
                     int eventid;
                     unType UN;
                     int bytes;
                     int parent;
                     int grandparent;
                     int ioflag;
                   };

struct eventstruct * event;

/*----------------------------------------------------------*/
/*    elapsed-time timing functions                         */
/*----------------------------------------------------------*/
#define WTIME(TV) gettimeofday(&TV, NULL)
#define TCONV(TV) (double) (TV).tv_sec + 1.0e-6*((double) (TV).tv_usec)

/*----------------------------------------------------------*/
/*    MPI version 3 uses const qualifiers                   */
/*----------------------------------------------------------*/
#if MPI_VERSION >= 3
typedef const void * sbuf_t;
typedef const int * iarray_t;
typedef const MPI_Datatype * datarray_t;
typedef const char * name_t;
#else
typedef void * sbuf_t;
typedef int * iarray_t;
typedef MPI_Datatype * datarray_t;
typedef char * name_t;
#endif

/*----------------------------------------------------------*/
/*    variables with file scope                             */
/*----------------------------------------------------------*/
static int taskid = 0;
static int ntasks = 1;
static int jobid = 0;
static char * trace_dir = NULL;
static double elapsed_time, elapsed_time_initial, mpi_init_time;
static double exclusive_time = 0.0, exclusive_time_start;
static double user_time, user_time_initial;
static double system_time, system_time_initial;
static long context_switches, context_switches_initial;
static double max_memory;
static int event_number = 0;
#if __BYTE_ORDER__ ==  __ORDER_LITTLE_ENDIAN__
static int swap_bytes = 0;  /* default value for swap_bytes */
#else
static int swap_bytes = 1;  /* default value for swap_bytes */
#endif
static int maxlen;
static int collect_summary = 1;
static int first_summary_start = 1;
static int summary_start_count = 0;
static int first_trace_start = 1;
static int trace_events = 0;
static int enable_tracing = 0;
static int time_window_tracing = 0;
static double trace_begin_time = 0.0;
static double trace_end_time = 0.0;
static int time_window_profiling = 0;
static double profile_begin_time = 0.0;
static double profile_end_time = 0.0;
static int traceback_level = 0; 
static int traceback_abort = 0; 
static int trace_send_pattern = 0;
static int translate_ranks = 1;
static float * send_bytes = NULL;
static int * send_count = NULL;
static MPI_Group world_group = MPI_GROUP_EMPTY;
static int save_all_tasks = 0;
static int output_filter = 0;
static int use_output_list = 0;
static int trace_max_rank = 255;
static int trace_min_rank = 0;
static int event_buffer_overflow = 0;
static int add_timestamp = 0;
static char timestamp[12];

static long event_count[MAX_IDS];
static double total_time[MAX_IDS];
static double total_bytes[MAX_IDS];

static char label[MAX_IDS][80];

static long bin_count[MAX_IDS][MAX_BINS];
static double bin_bytes[MAX_IDS][MAX_BINS];
static double bin_time[MAX_IDS][MAX_BINS];

static double total_size[MAX_BINS];
static int comm_size[MAX_BINS];
static long comm_count[MAX_IDS][MAX_BINS];
static double comm_bytes[MAX_IDS][MAX_BINS];
static double comm_time[MAX_IDS][MAX_BINS];

static int vprof_profile = 0;
static int pg_profile = 0;
static int comm_profile = 0;
static int collective_barrier = 0;
static int barrier_flag[MAX_IDS];
static double synctime = 0.0;

static char host[80];
static char * hostnames;

static int disable_tracing[MAX_IDS];

/*-----------------------------------------------*/
/* variables for profiling by the caller address */
/*-----------------------------------------------*/
#define FIFO_DEPTH 16
static int max_profile_blocks = 10000;
static int max_stack_depth = 4;
static int max_reported_stacks = 50;
static int profile_block = 0;
static int profile_by_call_stack= 0;
static int profile_fifo[FIFO_DEPTH];
static long ** profile_stack;
static long ** profile_call_count;
static unsigned int * profile_key;
static double * profile_elapsed_time;
static double ** profile_function_time;

/*----------------------------------------------------------*/
/*    function prototypes                                   */
/*----------------------------------------------------------*/
static void LogEvent(int, struct timeval, struct timeval, int, int, int, MPI_Comm);
static void LogIOEvent(int, struct timeval, struct timeval, long, int, MPI_Comm);
static void get_parents(int, int *, int *);
static void get_stack(int, unsigned int *, long *);
static inline unsigned int fletcher32(const uint16_t *, int);
static void write_tracefile(FILE *, struct eventstruct *, int);
static void reverse_byte_order(char *, char *, int);
static void swap8(char * in, char * out);
static void swap4(char * in, char * out);
static void write_profile_data(void);
static void write_profile_data_myrank(void);
static int index_from_address(int);
static int index_from_key(unsigned int);
static void print_profile_by_call_stack(FILE *);
static void initialize_summary_data(void);
static void stop_timers(void);

void mpitrace_traceback(int *);
void mpitrace_handler(MPI_Comm *comm, int *error);
void mpitrace_sortx(double *, int, int *, int);
void mpitrace_trace_start(void);
void mpitrace_trace_stop(void);
void mpitrace_summary_start(void);
void mpitrace_summary_stop(void);

void mpitrace_vprof_start(void);
void mpitrace_vprof_stop(void);
void mpitrace_vprof_write(int, int, char *);

#ifdef HPM
void HPM_Init(void);
void HPM_Start(const char *);
void HPM_Stop(const char *);
void HPM_Print(int, int, char *, int);
void HPM_Print_myrank(int, int, char *, int);
#endif
#ifdef HPMPROF
void HPM_Prof_init(void);
void HPM_Prof_Auto_start(void);
void HPM_Prof_start(void);
void HPM_Prof_stop(void);
void HPM_Prof_write(int, int, char *, int);
#endif
