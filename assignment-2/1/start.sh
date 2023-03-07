#!/bin/bash
#echo "Updating Python..."
#apt update && apt install python3 -y
cd $1
rm -r output
rm -r input
mkdir input
mkdir output
echo "=== Running input formatting script... ==="
python3 input_formatter.py
echo "=== Generating input files for mapreduce job... ==="
ls -l ./input
echo "=== removing old output and input from HDFS... ==="
hdfs dfs -rm -r output
hdfs dfs -rm -r input
echo "=== Creating input directory... ==="
hadoop fs -mkdir -p input
echo "=== Copying input files to HDFS... ==="
hdfs dfs -put input/* input
echo "=== Running MapReduce JOB1 ==="
mapred streaming -files mapper.py,reducer.py -mapper 'python3 mapper.py' -reducer 'python3 reducer.py' -input input -output output
echo "=== MapReduce job completed. ==="
rm -r input
mkdir input
echo "=== Preparing for JOB2, removing old input files ==="
hdfs dfs -cat output/* >input/f1.txt
echo "====================  STAGE 1 OUPUT  ===================="
cat input/f1.txt
echo "========================================================="
echo "=== removing old output and input from HDFS... ==="
hdfs dfs -rm -r output
hdfs dfs -rm -r input
echo "=== Creating input directory... ==="
hadoop fs -mkdir -p input
echo "=== Copying input files to HDFS... ==="
hdfs dfs -put input/* input
echo "=== Running MapReduce JOB2 ==="
mapred streaming -files mapper2.py,reducer2.py -mapper 'python3 mapper2.py' -reducer 'python3 reducer2.py' -input input -output output
echo "=== MapReduce job completed. ==="
hdfs dfs -cat output/* >output/out.txt
echo "====================  STAGE 2 OUPUT  ===================="
cat output/out.txt
echo "========================================================="
cd ..
