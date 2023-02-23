#!/bin/bash
#echo "Updating Python..."
#apt update && apt install python3 -y
rm -r output
rm -r input
mkdir input
mkdir output
echo "=== Running runner script... ==="
python3 runner.py
echo "=== Generating input files for mapreduce job... ==="
ls -l ./input
echo "=== removing old output and input from HDFS... ==="
hdfs dfs -rm -r output
hdfs dfs -rm -r input
echo "=== Creating input directory... ==="
hadoop fs -mkdir -p input
echo "=== Copying input files to HDFS... ==="
hdfs dfs -put input/* input
echo "=== Running MapReduce... ==="
mapred streaming -files mapper.py,reducer.py -mapper 'python3 mapper.py' -reducer 'python3 reducer.py' -input input -output output
echo "=== MapReduce job completed. ==="
