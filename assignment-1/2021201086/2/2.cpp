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
MPI_Datatype MPI_PARTICLE;

bool comp(Particle &a, Particle &b)
{
    if (a.x == b.x)
        return a.y <= b.y;
    return a.x < b.x;
}

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

void collisionResolution(unordered_map<int, vector<Particle>> &row_map, int lb, int ub, int rows, int cols, int k)
{
    unordered_map<string, vector<Particle *>> collision_points;
    for (int i = lb; i <= ub; i++)
    {
        for (Particle &p : row_map[i])
        {
            collision_points[parseLoc(p)].push_back(&p);
        }
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
                // cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'L' && p.second[1]->dir == 'R')
            {
                p.second[0]->dir = 'U';
                p.second[1]->dir = 'D';
                // cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'U' && p.second[1]->dir == 'D')
            {
                p.second[0]->dir = 'R';
                p.second[1]->dir = 'L';
                // cout << " <collided at " << p.first << "  >" << endl;
            }
            else if (p.second[0]->dir == 'D' && p.second[1]->dir == 'U')
            {
                p.second[0]->dir = 'L';
                p.second[1]->dir = 'R';
                // cout << " <collided at " << p.first << "  >" << endl;
            }
        }
    }
}
void outputResolver(vector<Particle> &particles, int n, int m, int k)
{
    vector<vector<string>> grid(m, vector<string>(n, ""));
    for (Particle &p : particles)
    {
        grid[m - p.y - 1][p.x].push_back(p.dir);
    }
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (grid[i][j].empty())
                cout << "-";
            else
                cout << grid[i][j];
            cout << "\t";
        }
        cout << endl;
    }
}
/**
 * @brief
 *
 * @param particles : Particle List
 * @param rank : recieving process rank
 */
void particle_send(vector<Particle> &particles, int rank)
{
    int size = particles.size();
    MPI_Send(&size, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
    if (size == 0)
        return;
    MPI_Send(particles.data(), particles.size(), MPI_PARTICLE, rank, 0, MPI_COMM_WORLD);
}
/**
 * @brief
 *
 * @param rank : Sending Process Rank
 * @return vector<Particle> : Gets list of Particles
 */
vector<Particle> particle_recv(int rank)
{
    int msg_size;
    // MPI_Status status;
    // MPI_Probe(rank, 0, MPI_COMM_WORLD, &status);
    // MPI_Get_count(&status, MPI_PARTICLE, &msg_size);
    MPI_Recv(&msg_size, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (msg_size == 0)
        return {};
    vector<Particle> particles(msg_size);
    MPI_Recv(particles.data(), msg_size, MPI_PARTICLE, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return particles;
}
void particle_filter(unordered_map<int, vector<Particle>> &row_map, int lb, int ub, int rank, int size, int iter)
{
    // send phase
    if (rank != 0) // down
    {
        // cout << "iter =" << iter << " to down send rank =" << rank << " msg size =" << row_map[lb - 1].size() << endl;
        particle_send(row_map[lb - 1], rank - 1);
        row_map.erase(lb - 1);
    }
    if (rank != size - 1) // up
    {
        // cout << "iter =" << iter << " to up send rank =" << rank << " msg size =" << row_map[ub + 1].size() << endl;
        particle_send(row_map[ub + 1], rank + 1);
        row_map.erase(ub + 1);
    }
    // recv phase
    if (rank != size - 1) // up recv
    {
        vector<Particle> buffer = particle_recv(rank + 1);
        // cout << "iter =" << iter << " from up recv rank =" << rank << " msg size =" << buffer.size() << endl;
        row_map[ub].insert(row_map[ub].end(), buffer.begin(), buffer.end());
    }
    if (rank != 0)
    {
        vector<Particle> buffer = particle_recv(rank - 1);
        // cout << "iter =" << iter << " from down recv rank =" << rank << " msg size =" << buffer.size() << endl;
        row_map[lb].insert(row_map[lb].end(), buffer.begin(), buffer.end());
    }
}

int main(int argc, char *argv[])
{
    // if stream remove later
    // ifstream cin("./assignment-1/2_inp.txt");
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // ----------------------------------------------------------------------------
    /* MPI PARTICLE : Custom PARTICLE TYPE */
    MPI_Datatype old_types[2];
    MPI_Aint offsets[2], lower_bound, extent;
    MPI_Type_get_extent(MPI_INT, &lower_bound, &extent);
    int blockcounts[2];
    blockcounts[0] = 3;
    old_types[0] = MPI_INT;
    offsets[0] = 0;
    blockcounts[1] = 1;
    old_types[1] = MPI_CHAR;
    offsets[1] = 2 * extent;
    MPI_Type_create_struct(1, blockcounts, offsets, old_types, &MPI_PARTICLE);
    MPI_Type_commit(&MPI_PARTICLE);
    // ----------------------------------------------------------------------------
    int params[4];
    // vector<Particle> particles;
    vector<vector<Particle>> particles;
    if (rank == MASTER)
    {
        cin >> params[0] >> params[1] >> params[2] >> params[3];
        // params[0] = 4, params[1] = 6, params[2] = 4, params[3] = 15;
        // int total = params[2] + size - ((params[2]) % size);
        // int elements = total / size;
        particles.resize(params[1]);
        for (int i = 0; i < params[2]; i++)
        {
            // Particle p;
            int x, y;
            char dir;
            cin >> x >> y >> dir;
            particles[y].push_back(Particle{x, y, dir});
        }
        // for (int i = 0; i < params[2]; i++)
        // {
        //     cout << particles[i].x << " " << particles[i].y << " " << particles[i].dir << endl;
        // }
    }
    double start_time;
    if (rank == MASTER)
    {
        start_time = MPI_Wtime();
    }
    MPI_Bcast(params, 4, MPI_INT, MASTER, MPI_COMM_WORLD);
    int n = params[0], m = params[1], k = params[2], t = params[3];
    if (m < size)
        size = m;
    int ROWS_PER_PROC = floor((double)m / (double)size);
    int EXTRA = size - m % size;
    if (!EXTRA)
        EXTRA = size;
    if (rank == MASTER)
    {
        int offset = 0;
        for (int i = 0; i < size; i++)
        {
            int BOUND[2];
            BOUND[0] = offset;
            BOUND[1] = offset + ROWS_PER_PROC - 1;
            if (i >= EXTRA)
                BOUND[1]++;
            offset = BOUND[1] + 1;
            MPI_Send(BOUND, 2, MPI_INT, i, 0, MPI_COMM_WORLD);
            vector<Particle> payload;
            for (int k = BOUND[0]; k <= BOUND[1]; k++)
            {
                // cout << k << endl;
                if (!particles[k].empty())
                {
                    payload.insert(payload.end(), particles[k].begin(), particles[k].end());
                }
            }
            // cout << "particle sent to process rank " << i << " no. of particles: " << payload.size() << endl;
            particle_send(payload, i);
        }
    }
    particles.clear();
    vector<Particle> list;
    if (rank < size)
    {
        unordered_map<int, vector<Particle>> row_map;
        vector<Particle> sub_list;
        int BOUND[2];
        MPI_Recv(BOUND, 2, MPI_INT, MASTER, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        int LOWER_BOUND = BOUND[0], UPPER_BOUND = BOUND[1];
        sub_list = particle_recv(MASTER);
        // cout << "|  rank =" << rank << " LB = " << LOWER_BOUND << " UB =" << UPPER_BOUND << endl;
        for (Particle &p : sub_list)
        {
            row_map[p.y].push_back(p);
            // cout << " recieved particle at rank = " << rank << "\t| \t" << p.x << "\t" << p.y << "\t" << p.dir << endl;
        }
        sub_list.clear();
        while (t--)
        {
            unordered_map<int, vector<Particle>> new_row_map;
            for (int i = LOWER_BOUND; i <= UPPER_BOUND; i++)
            {
                for (Particle &p : row_map[i])
                {
                    // cout << " << iter :" << t << " rank =" << rank << " " << p.x << " " << p.y << " " << p.dir << endl;
                    updateVector(p, m, n);
                    new_row_map[p.y].push_back(p);
                    // cout << "iter :" << t << " rank =" << rank << " " << p.x << " " << p.y << " " << p.dir << ">>" << endl;
                }
            }
            row_map = new_row_map;
            new_row_map.clear();
            particle_filter(row_map, LOWER_BOUND, UPPER_BOUND, rank, size, t);
            collisionResolution(row_map, LOWER_BOUND, UPPER_BOUND, m, n, k);
            // for (auto &v : row_map)
            // {
            //     for (Particle &p : v.second)
            //     {
            //         cout << "rank = " << rank << " iter = " << t << "\t" << p.x << " | " << p.y << " | " << p.dir << endl;
            //     }
            // }
            // MPI_Barrier(MPI_COMM_WORLD);
        }
        for (int i = LOWER_BOUND; i <= UPPER_BOUND; i++)
        {
            sub_list.insert(sub_list.end(), row_map[i].begin(), row_map[i].end());
        }
        row_map.clear();
        particle_send(sub_list, MASTER);
        sub_list.clear();
        // MPI_Barrier(MPI_COMM_WORLD);
    }
    MPI_Barrier(MPI_COMM_WORLD);
    if (rank == MASTER)
    {
        for (int i = 0; i < size; i++)
        {
            vector<Particle> buffer = particle_recv(i);
            list.insert(list.end(), buffer.begin(), buffer.end());
        }
        // cout << "--------------------------------------" << endl;
        // cout << n << " " << m << " " << k << endl;
        // cout << "--------------------------------------" << endl;
        // outputResolver(list, n, m, k);
        // cout << "--------------------------------------" << endl;
        sort(list.begin(), list.end(), comp);
        for (int i = 0; i < k; i++)
        {
            // Particle p;
            cout << list[i].x << " " << list[i].y << " " << list[i].dir << endl;
        }
        double end_time = MPI_Wtime();
        // cout << end_time - start_time << endl;
    }
    MPI_Finalize();
    return 0;
}