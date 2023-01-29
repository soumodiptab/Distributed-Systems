#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
const int MASTER = 0;
// Defines the stucture of the particle
typedef struct x
{
    int x;
    int y;
    char dir;
} Particle;

void updateVector(Particle &p, int rows, int cols)
{
    if (p.y == 0 && p.dir == 'D')
    {
        p.y = 1;
        p.dir = 'U';
        return;
    }
    else if (p.y == rows - 1 && p.dir == 'U')
    {
        p.y = rows - 2;
        p.dir = 'D';
        return;
    }
    else if (p.x == 0 && p.dir == 'L')
    {
        p.x = 1;
        p.dir = 'R';
        return;
    }
    else if (p.x == cols - 1 && p.dir == 'R')
    {
        p.x = cols - 2;
        p.dir = 'L';
        return;
    }
    else if (p.dir == 'U')
    {
        p.y++;
    }
    else if (p.dir == 'D')
    {
        p.y--;
    }
    else if (p.dir == 'L')
    {
        p.x--;
    }
    else if (p.dir == 'R')
    {
        p.x++;
    }
}
string parseLoc(Particle &p)
{
    return to_string(p.x) + "|" + to_string(p.y);
}

void collisionResolution(vector<Particle> &particles, int rows, int cols, int k)
{
    unordered_map<string, vector<Particle *>> collision_points;
    for (int i = 0; i < k; i++)
    {
        collision_points[parseLoc(particles[i])].push_back(&particles[i]);
    }
    for (auto &p : collision_points)
    {
        if (p.second[0]->x == 0 || p.second[0]->y == 0 || p.second[0]->x == cols - 1 || p.second[0]->y == rows - 1)
            continue;
        if (p.second.size() == 2)
        {
            if (p.second[0]->dir == 'R' && p.second[1]->dir == 'L')
            {
                p.second[0]->dir = 'D';
                p.second[1]->dir = 'U';
                cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'L' && p.second[1]->dir == 'R')
            {
                p.second[0]->dir = 'U';
                p.second[1]->dir = 'D';
                cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'U' && p.second[1]->dir == 'D')
            {
                p.second[0]->dir = 'R';
                p.second[1]->dir = 'L';
                cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'D' && p.second[1]->dir == 'U')
            {
                p.second[0]->dir = 'L';
                p.second[1]->dir = 'R';
                cout << " <collided at " << p.first << "  >" << endl;
            }
        }
    }
}
int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // ----------------------------------------------------------------------------

    /* MPI PARTICLE : Custom PARTICLE TYPE */
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
        cin >> params[0] >> params[1] >> params[2] >> params[3];
        // params[0] = 4, params[1] = 6, params[2] = 4, params[3] = 15;
        cout << params[0] << " " << params[1] << " " << params[2] << " " << params[3] << endl;
        int total = params[2] + size - ((params[2]) % size);
        int elements = total / size;
        particles.resize(params[2]);
        for (int i = 0; i < params[2]; i++)
        {
            // Particle p;
            cin >> particles[i].x >> particles[i].y >> particles[i].dir;
        }
        // particles[0].x = 1, particles[0].y = 1, particles[0].dir = 'R';
        // particles[1].x = 1, particles[1].y = 1, particles[1].dir = 'L';
        // particles[2].x = 3, particles[2].y = 4, particles[2].dir = 'U';
        // particles[3].x = 1, particles[3].y = 1, particles[3].dir = 'U';
        for (int i = 0; i < params[2]; i++)
        {
            // Particle p;
            cout << particles[i].x << " " << particles[i].y << " " << particles[i].dir << endl;
        }
    }
    // MPI_Bcast(params, 4, MPI_INT, MASTER, MPI_COMM_WORLD);
    int n = params[0], m = params[1], k = params[2], t = params[3];
    int scatter_count = ceil((double)k / size);
    // PARAM INIT
    while (t--) // for each time-slice
    {
        // Particle *sub_particles = (Particle *)malloc(sizeof(Particle) * scatter_count);
        // Particle sub_particles[scatter_count];
        // if (rank == MASTER)
        // {
        //     scatter_count = ceil((double)particles.size() / size);
        // }
        // MPI_Bcast(&scatter_count, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
        // vector<Particle> sub_particles(scatter_count);
        // MPI_Scatter(particles.data(), scatter_count, MPI_PARTICLE, sub_particles.data(), scatter_count, MPI_PARTICLE, MASTER, MPI_COMM_WORLD);
        collisionResolution(particles, m, n, k);
        for (int i = 0; i < k; i++)
        {
            // cout << rank << "\t" << sub_particles[i].x << "\t" << sub_particles[i].y << "\t" << sub_particles[i].dir << endl;
            updateVector(particles[i], m, n);
        }
        // MPI_Gather(sub_particles.data(), scatter_count, MPI_PARTICLE, particles.data(), scatter_count, MPI_PARTICLE, MASTER, MPI_COMM_WORLD);
        // if (rank == MASTER)
        // {
        //     cout << " gathered" << endl;
        // collision resolution
        // collisionResolution(particles, m, n, k);
        cout << " ------ " << endl;
        for (int i = 0; i < k; i++)
        {
            // Particle p;
            cout << "* " << particles[i].x << " " << particles[i].y << " " << particles[i].dir << endl;
        }
        //}
        // MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Finalize();
    return 0;
}