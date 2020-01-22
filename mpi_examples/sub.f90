! Copyright IBM Corporation, 2019
! author : Bob Walkup
!

subroutine sub()
  implicit none
  include 'mpif.h'
  integer myrank, val, info
  call mpi_comm_rank(mpi_comm_world, myrank, info)
  if (myrank == 0) then
    val = 1
  else
    val = -1
  end if
  call mpi_bcast(val, 1, mpi_integer, 0, mpi_comm_world, info)
end
