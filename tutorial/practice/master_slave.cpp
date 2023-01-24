#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

using namespace std;
void master()
{
}
void slave()
{
}
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if (rank == 0)
        master();
    else
        slave();
    MPI_Finalize();
    return 0;
}