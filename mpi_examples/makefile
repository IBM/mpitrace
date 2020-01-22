all : hi hello mixed

clean :
	rm -f *.o hi hello mixed

hi : hi.f90
	mpif90 -g hi.f90 -o hi 

hello : hello.c
	mpicc -g hello.c -o hello 

mixed : mixed.c sub.f90
	mpicc -c -g mixed.c
	mpif90 -c -g sub.f90
	mpif90 -g mixed.o sub.o -o mixed
