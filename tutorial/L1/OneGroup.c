#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
	// 0 - n-1  | each process will get a rank
	int rank, size;
	MPI_Init(&argc, &argv);
	//group of processes use the communicator to send messages to one another
	MPI_Comm comm = MPI_COMM_WORLD;
	int comm_size;
	char name[MPI_MAX_OBJECT_NAME];
	int name_length;

	MPI_Comm_size(comm, &comm_size);
	MPI_Comm_get_name(comm, name, &name_length);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	if (rank == 0)
		printf("The communicator %s contains %d MPI processes.\n", name, comm_size);
	printf("I am %d of %d \n", rank, size);
	MPI_Finalize();
	return 0;
}
