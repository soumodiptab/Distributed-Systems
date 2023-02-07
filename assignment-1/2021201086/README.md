# Distributed Systems: ASSIGNMENT 1

Directory structure
```
.
└──2021201086
   ├── 1
   │   └── 1.cpp
   ├── 2
   │   └── 2.cpp
   ├── 3
   │   └── 3.cpp
   └── readme.md
```
## Problem 1:  Mandelbrot Set (20 points)

### Time Complexity
T(n)=`O(((M * N)/P) * K)`
### Message Complexity
Message complexity =` O(P)` 
### Space Requirements
S(n)= `O(M*N)`

### Performance Scaling :
For Input 160 160 1000
```
real    0m0.611s \
user    0m0.375s\
sys     0m0.055s

real    0m0.474s \
user    0m0.454s \
sys     0m0.086s

real    0m0.514s \
user    0m0.617s \
sys     0m0.240s

real    0m0.463s \
user    0m0.675s \
sys     0m0.266s

real    0m0.441s \
user    0m0.814s \
sys     0m0.284s

real    0m0.465s \
user    0m0.971s \
sys     0m0.330s

real    0m0.444s \
user    0m1.114s \
sys     0m0.326s

real    0m0.447s \
user    0m1.244s \
sys     0m0.355s

real    0m0.432s \
user    0m1.387s \
sys     0m0.435s

real    0m0.401s \
user    0m1.196s \
sys     0m0.397s 

real    0m0.387s \
user    0m1.198s \
sys     0m0.406s

real    0m0.424s \
user    0m1.722s \
sys     0m0.465s

```

-----------------------------------------------------------------------------
## Problem 2: Tony Stark and Pym Particles (40 points)

### Time Complexity
T(n)=` O(K * T)`
### Message Complexity
* `MPI_PARTICLE`: Created a message which holds `x`,`y` and `direction` of the the particle.
* I am sending a vector of `MPI_PARTICLES` when two rows are interchanging information
* `O( P * size of particle)` where `P` is the no. of particles
### Space Requirements
* `S(n)=O(K)` where k is the no. of particles per process
### Performance Scaling
| no. of processes | Timetaken  | 
| ---------------- | ---------- | 
| 1                |0.00267175  |
| 2                |0.00156294  |
| 3                |0.00148828  |
| 4                |0.00128827  |
| 5                |0.00132342  |
| 6                |0.00127255  |
| 7                |0.00105989  |
| 8                |0.00104411  |
| 9                |0.00131021  |
| 10               |0.00142923  |
| 11               |0.00122936  |
| 12               |0.00203229  |
---------------------------------------------------------------------
## Problem 3:  Jar of Words (40 points)

### Approach :
1. Dividing the given array of freq and key into different processes and letting them sort using Distributed Merge sort using Tree Method to join between different processes.
2. Parallelizing each diagonal calculation to different processes.
3. Each process maintains its own table for optimal binary search tree and parent.
4. Each process calculates part of the diagonal and then uses `AllScatterV` to recombine and inform all processses of the diagonal calculation.
5. Each stage the diagonals decreases and at last we find the ans at the oth process.
6. At 0th process retrace parent

### Time complexity
* `n` : no. of nodes of the optimal binary search tree
* Merge sort time :`(n/worldsize)log(n/world_size)`
* Time complexity =
`O(n*(n/world_size + n-1/world_size + ... + 1/world_size)) = O(n^2)`
### Message Complexity
* `MPI_COST`:Using a Custom message to Send the details of the cost and parent of an index (i,j)
* `MPI_DATA`:data + freq of the Data item
* While sorting the array using distributed-tree mergesort sending a vector of each `MPI_DATA` : `O(n/WORLD_SIZE) * size of MPI_DATA`
* While collecting diagonals from all processes and scattering them to all process I am using vector of `MPI_COST`. So complexity = `O(diagonal_size / world_size) * size of MPI_COST` in each stage of the iteration.
### Space Requirements

* Using two 2d array for storing parent and optimal binary search tree cost
* using array to store data and freq

`S(n)=n^2 + 2 * n`
### Performance scaling

| no. of processes | Timetaken  | 
| ---------------- | ---------- | 
| 1                |7.5525e-05  |
| 2                |0.000136731 |
| 3                |0.000166117 |
| 4                |0.000234338 |
| 5                |0.000234247 |
| 6                |0.000217044 |
| 7                |0.000271348 |
| 8                |0.000296886 |
| 9                |0.000559658 |
| 10               |0.000480227 |
| 11               |0.000292037 |
| 12               |0.000432535 |