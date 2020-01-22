/* Copyright IBM Corporation, 2019
 * author : Bob Walkup
 */

#include <stdio.h>
#include <mpi.h>

void sub_(void);

int main(int argc, char * argv[])
{
  int myrank;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
  printf("hello from rank %d\n", myrank);
  sub_();
  MPI_Finalize();
  return 0;
}
