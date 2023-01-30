#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
typedef struct d
{
    int key;
    int freq;
} Data;

bool comp(const Data &a, const Data &b)
{
    return a.key <= b.key;
}
const int MASTER = 0;
int main(int argc, char *argv[])
{
    int n;
    /* MPI Data : freq + key */
    MPI_Datatype MPI_DATA, oldtypes[1];
    MPI_Aint offsets[1];
    int blockcounts[1];
    oldtypes[0] = MPI_INT;
    offsets[0] = 0;
    blockcounts[0] = 2;
    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &MPI_DATA);
    MPI_Type_commit(&MPI_DATA);
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    vector<Data> arr;
    if (rank == MASTER)
    {
        cin >> n;
        arr.resize(n);
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i].key >> arr[i].freq;
        }
    }
    // distributed - sort
    int scatter_count;
    if (rank == MASTER)
    {
        scatter_count = ceil((double)n / size);
    }
    MPI_Bcast(&scatter_count, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    vector<Data> sub_list(scatter_count);
    MPI_Scatter(arr.data(), scatter_count, MPI_DATA, sub_list.data(), scatter_count, MPI_DATA, MASTER, MPI_COMM_WORLD);
    
    MPI_Gather(sub_list.data(), scatter_count, MPI_PARTICLE, particles.data(), scatter_count, MPI_PARTICLE, MASTER, MPI_COMM_WORLD);
    if (rank == MASTER)
    {
        cout << " gathered" << endl;
    }
    MPI_Finalize();
    return 0;
}