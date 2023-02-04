/**
 * @file 3.cpp
 * @author Soumodipta Bose (sbose2019@gmail.com)
 * @brief
 * @version 1.0
 * @date 2023-02-05
 *
 * @copyright Copyright (c) 2023
 *
 */
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
MPI_Datatype MPI_DATA, MPI_COST;
/**
 * @brief
 *
 * @param arr
 * @param i
 * @param j
 * @return int
 */
int getsum(vector<Data> &arr, int i, int j)
{
    int sum = 0;
    for (int k = i; k <= j; k++)
    {
        sum += arr[k].freq;
    }
    return sum;
}
/**
 * @brief
 *
 * @param dp
 * @param parent
 * @param send_buffer
 * @param rank
 * @param size
 * @param n
 * @param stage_datacount
 * @param extra
 */
void progress_transfer(vector<vector<int>> &dp, vector<vector<int>> &parent, vector<Cost> send_buffer, int rank, int size, int n, int stage_datacount, int extra)
{
    int datacount[size] = {0};
    int displacements[size] = {0};
    int offset = 0;
    for (int i = 0; i < size; i++)
    {
        if (offset >= n)
            break;
        if (n < size)
            datacount[i] = 1;
        else
            datacount[i] = stage_datacount + (i >= extra);
        displacements[i] = offset;
        offset += datacount[i];
        cout << "stage = " << n << " progress = " << datacount[i] << " " << displacements[i] << endl;
    }
    cout << "stage =" << n << " offset =" << offset << " << " << endl;
    vector<Cost> recieve_buffer(offset);
    // MPI_Barrier(MPI_COMM_WORLD);
    MPI_Allgatherv(send_buffer.data(), send_buffer.size(), MPI_COST, recieve_buffer.data(), datacount, displacements, MPI_COST, MPI_COMM_WORLD);
    n--;
    for (Cost &c : recieve_buffer)
    {
        dp[c.i][c.j] = c.cost;
        parent[c.i][c.j] = c.parent;
        if (rank == MASTER)
        {
            cout << rank << " stage =" << n << " || [" << c.i << "," << c.j << "] = " << c.cost << "|";
        }
    }
    cout << endl;
}
/**
 * @brief
 *
 * @param ans
 * @param arr
 * @param parent
 * @param i
 * @param j
 * @param prev
 */
void recur(vector<int> &ans, vector<Data> &arr, vector<vector<int>> &parent, int i, int j, int prev)
{
    if (i > j)
        return;
    // cout << "< " << i << "|" << j << "|" << prev << " >" << endl;
    int current = parent[i][j];
    ans[current] = prev + 1;
    recur(ans, arr, parent, i, current - 1, current);
    recur(ans, arr, parent, current + 1, j, current);
}
/**
 * @brief
 *
 * @param parent
 * @param arr
 * @return vector<int>
 */
vector<int> construct_tree(vector<vector<int>> &parent, vector<Data> &arr)
{
    int n = parent.size();
    vector<int> ans(n, 0);
    recur(ans, arr, parent, 0, n - 1, -1);
    return ans;
}
/**
 * @brief
 *
 * @param dataitems
 * @param rank
 */
void data_send(vector<Data> &dataitems, int rank)
{
    int size = dataitems.size();
    MPI_Send(&size, 1, MPI_INT, rank, 0, MPI_COMM_WORLD);
    if (size == 0)
        return;
    MPI_Send(dataitems.data(), dataitems.size(), MPI_DATA, rank, 0, MPI_COMM_WORLD);
}
/**
 * @brief
 *
 * @param rank
 * @return vector<Data>
 */
vector<Data> data_recv(int rank)
{
    int msg_size;
    MPI_Recv(&msg_size, 1, MPI_INT, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    if (msg_size == 0)
        return {};
    vector<Data> dataitems(msg_size);
    MPI_Recv(dataitems.data(), msg_size, MPI_DATA, rank, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    return dataitems;
}
/**
 * @brief Scatter data across different processes using variable displacement size
 *
 * @param rank - rank of process
 * @param world_size - size of the processes
 * @param data_items - global data to be scattered
 */
vector<Data> data_scatter(int rank, int world_size, vector<Data> &data_items)
{
    int n = data_items.size();
    int alloc = n / world_size;
    int extra = world_size - (n % world_size);
    if (rank == MASTER)
    {
        int offset = 0;
        for (int i = 0; i < world_size; i++)
        {
            int start = offset;
            int end = start + alloc + (i >= extra);
            offset = end;
            vector<Data> buffer(data_items.begin() + start, data_items.begin() + end);
            data_send(buffer, i);
        }
    }
    vector<Data> recv_buffer = data_recv(MASTER);
    return recv_buffer;
}
/**
 * @brief
 *
 * @param rank
 * @param world_size
 * @param data_items
 * @return vector<Data>
 */
vector<Data> data_gather(int rank, int world_size, vector<Data> &data_items)
{
    vector<Data> buffer;
    data_send(data_items, MASTER);
    if (rank == MASTER)
    {
        for (int i = 0; i < world_size; i++)
        {
            vector<Data> temp_buffer = data_recv(i);
            buffer.insert(buffer.end(), temp_buffer.begin(), temp_buffer.end());
        }
    }
    return buffer;
}
/**
 * @brief
 *
 * @param half1
 * @param half2
 * @return vector<Data>
 */
vector<Data> merge(vector<Data> &half1, vector<Data> &half2)
{
    vector<Data> res(half1.size() + half2.size());
    int l = 0, r = 0, ctr = 0;
    while (l < half1.size() && r < half2.size())
    {
        if (half1[l].key <= half2[r].key)
            res[ctr++] = half1[l++];
        else
            res[ctr++] = half2[r++];
    }
    while (l < half1.size())
        res[ctr++] = half1[l++];
    while (r < half2.size())
        res[ctr++] = half2[r++];
    return res;
}
/**
 * @brief
 *
 * @param height
 * @param rank
 * @param local
 * @param global
 */
void mergesort(int height, int rank, int size, vector<Data> &local, vector<Data> &global)
{
    int parent, child;
    sort(local.begin(), local.end(), comp);
    int proc_height = 0;
    while (proc_height < height)
    {
        parent = (rank & ~(1 << proc_height));
        cout << "[Mergesort][rank = " << rank << " ]\tparent = " << parent << "\tproc_height = " << proc_height << endl;
        if (rank == parent)
        {
            child = (rank | (1 << proc_height));
            if (child < size)
            {
                vector<Data> child_data = data_recv(child);
                cout << "[Mergesort][rank = " << rank << " ][recv]\tparent = " << parent << "\tproc_height = " << proc_height << "child =" << child << "\trecv size =" << child_data.size() << endl;
                local = merge(local, child_data);
            }
            proc_height++;
        }
        else
        {
            cout << "[Mergesort][rank = " << rank << " ][send]\tparent = " << parent << "\tproc_height = " << proc_height << "\t send size = " << local.size() << endl;
            data_send(local, parent);
            if (proc_height != 0)
                local.clear();
            proc_height = height;
        }
    }
    if (rank == MASTER)
    {
        global = local;
        local.clear();
    }
}
/**
 * @brief
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char *argv[])
{
    // custom input
    ifstream cin("./assignment-1/3_inp.txt");
    // remove later
    int n, total;
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    /* MPI Data : freq + key */
    MPI_Datatype oldtypes[1], oldtypes1[1];
    MPI_Aint offsets[1], offsets1[1];
    int blockcounts[1], blockcounts1[1];
    oldtypes[0] = MPI_INT;
    offsets[0] = 0;
    blockcounts[0] = 2;
    MPI_Type_create_struct(1, blockcounts, offsets, oldtypes, &MPI_DATA);
    MPI_Type_commit(&MPI_DATA);
    /* MPI Cost : i,j,cost,parent */
    oldtypes1[0] = MPI_INT;
    offsets1[0] = 0;
    blockcounts1[0] = 4;
    oldtypes1[0] = MPI_INT;
    MPI_Type_create_struct(1, blockcounts1, offsets1, oldtypes1, &MPI_COST);
    MPI_Type_commit(&MPI_COST);
    if (rank == MASTER)
    {
        cin >> n;
        total = n;
    }
    MPI_Bcast(&n, 1, MPI_INT, MASTER, MPI_COMM_WORLD);
    vector<Data> arr(n);
    if (rank == MASTER)
    {
        for (int i = 0; i < n; i++)
        {
            cin >> arr[i].key >> arr[i].freq;
        }
        // arr = {
        //     {1, 2},
        //     {2, 3},
        //     {3, 4},
        //     {4, 1}};
    }

    int height = ceil(log2(size));
    cout << "<>Height =" << height << endl;
    // Parallelized merge sort code goes here
    vector<Data> sub_list = data_scatter(rank, size, arr);
    // for (auto &d : sub_list)
    // {
    //     cout << "rank =" << rank << "recieved  =" << d.freq << " | " << d.key << endl;
    // }
    if (rank == MASTER)
    {
        mergesort(height, rank, size, sub_list, arr);
    }
    else
    {
        mergesort(height, rank, size, sub_list, arr);
        arr.clear();
    }
    // MPI_Gather(sub_list.data(), scatter_count, MPI_DATA, arr.data(), scatter_count, MPI_DATA, MASTER, MPI_COMM_WORLD);
    vector<vector<int>> dp(n, vector<int>(n, 0));
    vector<vector<int>> parent(n, vector<int>(n, 0));
    if (rank == MASTER)
    {
        // sort(arr.begin(), arr.end(), comp);
        cout << "--------------------------------------------------------" << endl;
        for (Data &i : arr)
            cout << i.key << "|" << i.freq << endl;
        cout << endl;
        cout << "--------------------------------------------------------" << endl;
    }
    MPI_Bcast(arr.data(), n, MPI_DATA, MASTER, MPI_COMM_WORLD);
    for (int i = 0; i < n; i++)
    {
        dp[i][i] = arr[i].freq;
        parent[i][i] = i;
    }
    MPI_Barrier(MPI_COMM_WORLD);
    int chain_length = 2;
    n--;
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
        vector<Cost> send_buffer;
        if (rank < n) // atleast 1 data item to be computed per process
        {
            // cout << rank << " " << chain_length << " << start indices :" << start << " | " << end << endl;
            //   optimal bst code
            for (int i = start; i <= end; i++)
            {
                int j = i + chain_length - 1;
                // cout << rank << " " << chain_length << " << " << i << " | " << j << endl;
                dp[i][j] = INT_MAX;
                int offset_sum = getsum(arr, i, j);
                for (int r = i; r <= j; r++)
                {
                    // c = cost when keys[r] becomes root of this subtree
                    int c = ((r > i) ? dp[i][r - 1] : 0) +
                            ((r < j) ? dp[r + 1][j] : 0) +
                            offset_sum;
                    if (c < dp[i][j])
                    {
                        dp[i][j] = c;
                        parent[i][j] = r;
                    }
                }
                send_buffer.push_back({i, j, dp[i][j], parent[i][j]});
                cout << "rank =" << rank << "\t stage =" << n << "\t range =" << start << " " << end << "\t frame ="
                     << stage_datacount << "|" << extra << "\t|| cost[" << i << "," << j << "]=\t" << dp[i][j] << " | " << parent[i][j] << endl;
            }
        }
        // else
        //  cout << "rank =" << rank << "\t stage =" << n << endl;
        progress_transfer(dp, parent, send_buffer, rank, size, n, stage_datacount, extra);
        MPI_Barrier(MPI_COMM_WORLD);
        n--, chain_length++;
    }
    if (rank == MASTER)
    {
        cout << "-----------------------------------------------------------------" << endl;
        cout << "Final Answer = " << dp[0][total - 1] << endl;
        cout << "-----------------------------------------------------------------" << endl;
        for (auto &vec : parent)
        {
            for (int &ele : vec)
            {
                cout << ele << " ";
            }
            cout << endl;
        }
        vector<int> ans = construct_tree(parent, arr);
        for (int i = 0; i < total; i++)
        {
            cout << " key = " << arr[i].key << " parent = " << ans[i] << endl;
        }
        cout << "-----------------------------------------------------------------" << endl;
        // print output here
    }
    MPI_Finalize();
    return 0;
}