# Distributed Systems  | Assignment 2 | 2021201086

Map-reduce using Hadoop
## Instructions to execute runner script:
```bash
cd <rollno_question>
./runner_script.sh <JAR_FILE_PATH> <INPUT_FILE> <HDFS_INPUT_DIR> <HDFS_OUTPUT_DIR> <MAP_REDUCE_DIR>
```
* `JAR_FILE_PATH`: Location where the hadoop streaming jar file is stored
* `INPUT_FILE`: Input file for the problem with format as per the assignment pdf
* `HDFS_INPUT_DIR`: HDFS dir to store the input temporarily
* `HDFS_OUTPUT_DIR`: HDFS dir to store the ouput
* `MAP_REDUCE_DIR`: Directory where all map-reduce files and input formatting files are stored.

`Note:` I delete the HDFS dirs during multiple jobs of my runner script
## Question 1 : National Party

```bash
.
├── input_formatter.py
├── mapper.py
├── mapper2.py
├── reducer.py
├── reducer2.py
└── runner_script.sh
```
### Input Formatter :
File: `input_formatter.py`

Formats the input into form recognizable by JOB 1 mapper


### JOB 1 : Zone to State Aggregation

#### Mapper 1: 
File: `mapper.py`

Formats the input and puts state as the key for the reducer.
#### Reducer 1:
File: `reducer.py`

Sums zones with the same state and creates output to be used in next job
### JOB 2: State to Country Aggregation

#### Mapper 2:
File: `mapper2.py`

Formats the input and puts country as the key for the reducer.
#### Reducer 2:
File: `reducer2.py`

Reformats input to use country as key using the key 0 to ensure all blocks go to same reducer.

### Scaling :
The Solution has been divided into two jobs in such a manner that it can scale with multiple map and reducer worker nodes and not cause inconsistency.


---

## Questiion 2 : Matrix Multiplication
```bash
.
├── input_formatter.py
├── mapper.py
├── mapper2.py
├── reducer.py
├── reducer2.py
├── runner_script.sh
└── variables
```
### Input Formatter :
File: `input_formatter.py`

* Formats the input into sparse matrix form recognizable by JOB 1 mapper and uses `a` and `b` to keep track of matrix.
* Uses notation particlular`<a,i,j,val>` for `a` and `<b,j,k,val>` for `b`. 
* Stores dimensions of final matrix in `variables` file which is sourced by `runner_script`
* incase row and col donot match the script exits with `exit(1)` triggering `runner_script` to exit.

### JOB 1 :
This job fo

#### Mapper 1:
File: `mapper.py`

Formats the input and puts `j` as the key for the reducer such that values from `a` with same `j` and values from `b` with same `j` end up in the same `reducer`.

#### Reducer 1:
File: `reducer.py`

* First splits the records into different lists for matrix `a` and `b`.
* Multiplies a record of `a` `<i,j,val_a>` with `b` `<j,k,val_b>` to form `i,k : val_a * val_b`
* The output is used in the next cycle that is in JOB 2.
### JOB 2:

### Mapper 2:
File: `mapper2.py`

* Reformats the input and sends it to the same reducer in the format `<0,i,k,val>` where key is `0` i,e. same for all records.
* This ensures they end up in same `reducer`.

### Reducer 2:
File: `reducer2.py`

* Takes final dimensions as input parameter `ROW` and `COL` which are intelligently passed by `runner_script`.
* Creates an empty matrix.
* Sums all the records with the same `<i,k>` index.

### Scaling

* The solution is beautifully crafted, and takes into account the number of mapper and reducer and scales really well any number of mapper and reducer.
* JOB 2 ensures even if we use multi-reducers all the records end up in same reducer.