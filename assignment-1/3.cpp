#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
typedef struct d
{
    int key;
    int freq;
} Data;
typedef struct x
{
    int i;
    int j;
    int cost;
    int parent;
} Cost;

bool comp(const Data &a, const Data &b)
{
    return a.key <= b.key;
}
const int MASTER = 0;
MPI_Datatype MPI_COST;
int getsum(vector<Data> &arr, int i, int j)
{
    int sum = 0;
    for (int k = i; k <= j; k++)
    {
        sum += arr[k].freq;
    }
    return sum;
}

void progress_transfer(vector<vector<int>> &dp, vector<vector<int>> &parent, int rank, int size, int n, int start, int end, int stage_datacount)
{
    vector<Cost> send_buffer;
    vector<Cost> recieve_buffer;
    int datacount[size] = {0};
    int displacements[size] = {0};
    for (int i = 0; i < size)
    {
        datacount =
    }
    MPI_Allgatherv(send_buffer.data(), stage_datacount, MPI_COST, recieve_buffer.data(), datacount, displacements, MPI_COST, MPI_COMM_WORLD);
    n--;
    for (Cost &c : recieve_buffer)
    {
        dp[c.i][c.j] = c.cost;
        parent[c.i][c.j] = c.parent;
    }
}
int main(int argc, char *argv[])
{
    // custom input
    ifstream cin("./assignment-1/3_inp.txt");
    int n;
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    /* MPI Data : freq + key */
    MPI_Datatype MPI_DATA, oldtypes[1], oldtypes1[1];
    MPI_Aint offsets[1], offsets1[1];
    int blockcounts[1], blockcounts1[1];
    oldtypes[0] = MPI_INT;
    offsets[0] = 0;
    blockcounts[0] = 2;
    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &MPI_DATA);
    MPI_Type_commit(&MPI_DATA);
    /* MPI Cost : i,j,cost,parent*/
    oldtypes1[0] = MPI_INT;
    offsets1[0] = 0;
    blockcounts1[0] = 4;
    oldtypes1[0] = MPI_INT;
    MPI_Type_create_struct(1, blockcounts1, offsets1, oldtypes1, &MPI_COST);
    MPI_Type_commit(&MPI_COST);
    vector<Data> arr;
    if (rank == MASTER)
    {
        cin >> n;
        arr.resize(n);
        for (int i = 0; i < n; i++)
        {
            // cin >> arr[i].key >> arr[i].freq;
        }
    }
    arr = {{10, 34},
           {12, 8},
           {20, 50}};
    // distributed - sort
    // int scatter_count;
    // if (rank == MASTER)
    // {
    //     scatter_count = ceil((double)n / size);
    // }
    MPI_Bcast(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    /*
    Parallelized merge sort code goes here
    */
    // vector<Data> sub_list(scatter_count);
    // MPI_Scatter(arr.data(), scatter_count, MPI_DATA, sub_list.data(), scatter_count, MPI_DATA, MASTER, MPI_COMM_WORLD);
    // sort(sub_list.begin(), sub_list.end(), comp);
    // MPI_Gather(sub_list.data(), scatter_count, MPI_DATA, arr.data(), scatter_count, MPI_DATA, MASTER, MPI_COMM_WORLD);
    vector<vector<int>> dp(n, vector<int>(n, 0));
    vector<vector<int>> parent(n, vector<int>(n, 0));
    if (rank == MASTER)
    {
        sort(arr.begin(), arr.end(), comp);
    }
    MPI_Bcast(&arr, n, MPI_DATA, MASTER, MPI_COMM_WORLD);
    for (int i = 0; i < n; i++)
    {
        dp[i][i] = arr[i].freq;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int chain_length = 2;
    while (n)
    {
        // for each stage calculate the start,end indices and data items
        int stage_datacount = n / size;
        int extra = size - (n % size);
        int start, end;
        if (n < size) // edge case for when data count < world size
        {
            start = rank;
            end = rank;
        }
        else if (rank >= extra) // last processes get additional 1 data
        {
            start = rank * stage_datacount + (rank - extra);
            end = start + stage_datacount;
        }
        else // first processes get equal data
        {
            start = stage_datacount * rank;
            end = start + stage_datacount - 1;
        }
        if (rank < n) // atleast 1 data item to be computed per process
        {
            for (int i = start; i <= end - chain_length + 1; i++)
            {
                int j = i + chain_length - 1;
                dp[i][j] = INT_MAX;
                int offset_sum = getsum(arr, i, j);
                for (int r = i; r <= j; r++)
                {
                    // c = cost when keys[r] becomes root of this subtree
                    int c = ((r > i) ? dp[i][r - 1] : 0) +
                            ((r < j) ? dp[r + 1][j] : 0) +
                            offset_sum;
                    if (c < dp[i][j])
                        dp[i][j] = c;
                }
                cout << "rank =" << rank << "\t stage =" << n << "\t range =" << start << " " << end << "\t frame =" << stage_datacount << "|" << extra << "\t|| cost" << endl;
            }
        }
        // else
        //  cout << "rank =" << rank << "\t stage =" << n << endl;
        MPI_Barrier(MPI_COMM_WORLD);
        n--, chain_length++;
    }
    if (rank == MASTER)
    {
        // print output here
    }
    MPI_Finalize();
    return 0;
}