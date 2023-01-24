#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
const int MASTER = 0;
// Defines the stucture of the particle
typedef struct p
{
    int x;
    int y;
    char dir;
} Particle;

void updateVector(Particle &p, int rows, int cols)
{
    
}

void collisionResolution(vector<Particle> &particles, int rows, int cols)
{
}
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // data type declare ----------------------------------------------------------
    MPI_Datatype MPI_PARTICLE, oldtypes[2];
    MPI_Aint offsets[2], lb, extent;
    int blockcounts[2];
    oldtypes[0] = MPI_INT;
    offsets[0] = 0;
    blockcounts[0] = 3;
    MPI_Type_get_extent(MPI_INT, &lb, &extent);
    oldtypes[1] = MPI_CHAR;
    offsets[1] = 2 * extent;
    blockcounts[1] = 1;
    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &MPI_PARTICLE);
    MPI_Type_commit(&MPI_PARTICLE);
    // ----------------------------------------------------------------------------
    int params[4];
    vector<Particle> particles;
    if (rank == MASTER)
    {
        // cin >> params[0] >> params[1] >> params[2] >> params[3];
        params[0] = 5, params[1] = 5, params[2] = 3, params[3] = 1;
        int total = params[2] + size - ((params[2]) % size);
        int elements = total / size;
        particles.resize(params[2]);
        // for (int i = 0; i < params[2]; i++)
        // {
        //     // Particle p;
        //     cin >> particles[i].x >> particles[i].y >> particles[i].dir;
        // }
        particles[0].x = 0, particles[0].y = 0, particles[0].dir = 'U';
        particles[1].x = 0, particles[1].y = 1, particles[1].dir = 'D';
        particles[2].x = 1, particles[2].y = 0, particles[2].dir = 'L';
        for (int i = 0; i < params[2]; i++)
        {
            // Particle p;
            cout << particles[i].x << " " << particles[i].y << " " << particles[i].dir << endl;
        }
    }
    MPI_Bcast(params, 4, MPI_INT, 0, MPI_COMM_WORLD);
    int n = params[0], m = params[1], k = params[2], t = params[3];
    int scatter_count = ceil(((double)k) / (double)size);
    // PARAM INIT
    while (t--) // for each time-slice
    {
        // Particle *sub_particles = (Particle *)malloc(sizeof(Particle) * scatter_count);
        // Particle sub_particles[scatter_count];
        vector<Particle> sub_particles(scatter_count);
        MPI_Scatter(particles.data(), scatter_count, MPI_PARTICLE, sub_particles.data(), scatter_count, MPI_PARTICLE, MASTER, MPI_COMM_WORLD);
        // for (int i = 0; i < scatter_count; i++)
        // {
        //     cout << rank << "\t" << sub_particles[i].x << "\t" << sub_particles[i].y << "\t" << sub_particles[i].dir << endl;
        // }
        

        MPI_Gather(sub_particles.data(), scatter_count, MPI_PARTICLE, particles.data(), scatter_count, MPI_PARTICLE, MASTER, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        if (rank == MASTER)
        {
            cout << " gathered" << endl;
            cout << sizeof(MPI_PARTICLE) << " " << sizeof(Particle) << endl;
            for (int i = 0; i < params[2]; i++)
            {
                // Particle p;
                cout << " * " << particles[i].x << " " << particles[i].y << " " << particles[i].dir << endl;
            }
        }
    }
    if (rank == MASTER)
    {
        cout << t << " here" << endl;
        // delete (particles);
    }
    MPI_Finalize();
    return 0;
}