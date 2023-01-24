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
    int params[4];
    vector<Particle> particles;
    int scatter_count;
    if (rank == MASTER)
    {
        cin >> params[0] >> params[1] >> params[2] >> params[3];
        for (int i = 1; i <= params[2]; i++)
        {
            Particle p;
            cin >> p.x >> p.y >> p.dir;
            particles.push_back(p);
        }
        // params[0] = 20, params[1] = 16, params[2] = 1000;
    }
    MPI_Bcast(params, 4, MPI_INT, 0, MPI_COMM_WORLD);
    int n = params[0], m = params[1], k = params[2], t = params[3];
    scatter_count = ceil(((double)k) / (double)size);
    cout << rank << n << m << endl;
    // PARAM INIT
    while (t--) // for each time-slice
    {
        vector<Particle> sub_particles;
        MPI_Scatter(particles.data(), scatter_count * sizeof(Particle), MPI_BYTE, sub_particles.data(), scatter_count * sizeof(Particle), MPI_BYTE, MASTER, MPI_COMM_WORLD);
        for (Particle &p : sub_particles)
        {
            cout << rank << " " << p.x << " " << p.y << " " << p.dir << endl;
        }
        MPI_Gather(sub_particles.data(), scatter_count * sizeof(Particle), MPI_BYTE, particles.data(), scatter_count * sizeof(Particle), MPI_BYTE, MASTER, MPI_COMM_WORLD);
        // MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}