#include <bits/stdc++.h>
#include <mpi.h>
using namespace std;
const int MASTER = 0;
const double X_START = -1.5;
const double X_FIN = 1;
const double Y_START = -1;
const double Y_FIN = 1;
complex<double> translate(int idx, int rows, int cols)
{
    int row = idx / cols;
    int col = idx % cols;
    double dx = (X_FIN - X_START) / (cols - 1);
    double dy = (Y_FIN - Y_START) / (rows - 1);
    complex<double> val(X_START + col * dx, Y_START + row * dy);
    return val;
}
int mandelbrot(int idx, int rows, int cols, int k)
{
    complex<double> c = translate(idx, rows, cols);
    complex<double> z(0, 0);
    while (k--)
    {
        z = z * z + c;
        if (abs(z) > 2)
            return 0;
    }
    return 1;
}
int main(int argc, char *argv[])
{
    ifstream cin("./assignment-1/1_inp.txt");
    MPI_Init(&argc, &argv);
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int params[3];
    if (rank == MASTER)
    {
        cin >> params[0] >> params[1] >> params[2];
        // params[0] = 20, params[1] = 16, params[2] = 1000;
    }
    MPI_Bcast(params, 3, MPI_INT, 0, MPI_COMM_WORLD);
    int m = params[0], n = params[1], k = params[2];
    vector<int> flattened;
    if (rank == MASTER)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                flattened.push_back(i * m + j);
            }
        }
        if (flattened.size() % size != 0)
        {
            int dummy = size - flattened.size() % size;
            while (dummy--)
            {
                flattened.push_back(-1);
            }
        }
    }
    // get element to transfer
    int total = n * m + size - ((n * m) % size);
    int elements = total / size;
    vector<int> flat_sub(elements);
    MPI_Scatter(flattened.data(), elements, MPI_INT, flat_sub.data(), elements, MPI_INT, MASTER, MPI_COMM_WORLD);
    vector<int> sol_sub;
    for (int &idx : flat_sub)
    {
        if (idx == -1)
        {
            sol_sub.push_back(-1);
            continue;
        }
        sol_sub.push_back(mandelbrot(idx, n, m, k));
    }
    vector<int> sol(total);
    MPI_Gather(sol_sub.data(), elements, MPI_INT, sol.data(), elements, MPI_INT, MASTER, MPI_COMM_WORLD);
    if (rank == MASTER)
    {
        int idx = 0;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < m; j++)
            {
                cout << sol[idx++] << " ";
            }
            cout << endl;
        }
    }
    MPI_Finalize();
    return 0;
}