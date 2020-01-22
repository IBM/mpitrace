! Copyright IBM Corporation, 2019
! author : Bob Walkup
!

program main
  implicit none
  include 'mpif.h'
  integer myrank, info
  call mpi_init(info)
  call mpi_comm_rank(mpi_comm_world, myrank, info)
  print *, 'hi from rank ', myrank
  call mpi_finalize(info)
end
