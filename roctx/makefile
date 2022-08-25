#-------------------------------------------------------------
# optionally add an underscore to all Fortran entry points
#-------------------------------------------------------------
 FNAME = 
#FNAME = -DADD_UNDERSCORE

#-------------------------------------------------------------
# normally use gcc to build the wrapper libraries
# add -DHAS_INIT_THREAD if your MPI implementation supports MPI_Init_thread()
#-------------------------------------------------------------
CC = mpicc
ROCM_INC = -I /opt/rocm-5.0.0-rel19/include -I /opt/rocm-5.0.0-rel19/include/roctracer
ROCM_LIB = -L /opt/rocm-5.0.0-rel19/lib -lroctracer64 -lroctx64 -Wl,-rpath=/opt/rocm-5.0.0-rel19/lib
CFLAGS = -g -O -fPIC $(VPROF) -I. $(MPI_INC) $(ROCM_INC) -DUSE_ROCTX  -DHAS_INIT_THREAD

#------------------------------------------
# targets
#------------------------------------------
default: shared #static 

static :    libmpitrace.a

shared :  libmpitrace.so 

clean :
	rm -f *.a *.o *.so *.lst 

distclean :
	rm -f *.a *.o *.so *.lst makefile

#------------------------------------------------------------------
# rules for the wrapper libraries - add support for vprof profiling
#------------------------------------------------------------------
libmpitrace.a : mpitrace.o mpitrace_sortx.o      $(VPROF_STATIC_OBJ)
	ar crv $@ mpitrace.o mpitrace_sortx.o    $(VPROF_STATIC_OBJ)

libmpitrace.so : mpitrace.o mpitrace_sortx.o                             $(VPROF_DYNAMIC_OBJ)
	$(CC) -o libmpitrace.so   -shared mpitrace.o   mpitrace_sortx.o  $(VPROF_DYNAMIC_OBJ) $(FLIB) $(BFD_LIB) $(ROCM_LIB)

#------------------------------------------
# rules for .o files
#------------------------------------------
mpitrace.o : mpitrace.c mpitrace_common.c  init_part1.c init_part2.c \
              fortran_wrappers.c mpitrace.h
	$(CC) -c $(CFLAGS) mpitrace.c -o $@

mpitrace_sortx.o : mpitrace_sortx.c
	$(CC) -c $(CFLAGS) mpitrace_sortx.c -o $@

