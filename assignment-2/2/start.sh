#!/bin/bash
#echo "Updating Python..."
#apt update && apt install python3 -y
echo "removing old output... and input..."
hdfs dfs -rm -r output
hdfs dfs -rm -r input
echo "Creating input directory..."
hadoop fs -mkdir -p input
echo "Copying input files to HDFS..."
hdfs dfs -put input/* input
echo "Running MapReduce..."
mapred streaming -files mapper.py,reducer.py -mapper 'python3 mapper.py' -reducer 'python3 reducer.py' -input input -output output