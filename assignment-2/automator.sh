#!/bin/bash

JAR_FILE="/opt/hadoop-3.2.1/share/hadoop/tools/lib/hadoop-streaming-3.2.1.jar"
HDFS_INPUT_DIR="input"
HDFS_OUTPUT_DIR="output"
MAPREDUCE_DIR=$1
INPUT_FILE="inp.txt"
echo "=== Executing program : $1 ==="
echo "=== Removing old input ==="
docker exec -it namenode bash -c "rm -r $1"
echo "=== Copying program : $1 to namenode ==="
# docker cp mapper.py namenode:mapper.py
# docker cp reducer.py namenode:reducer.py
# docker cp input namenode:input
# docker cp start.sh namenode:start.sh
docker cp $1 namenode:$1
echo "=== All files copied to namenode. ==="
echo "=== Running start.sh on namenode. ==="
docker exec -it namenode bash -c "./$1/runner_script.sh $JAR_FILE $INPUT_FILE $HDFS_INPUT_DIR $HDFS_OUTPUT_DIR $MAPREDUCE_DIR"
cd ..
echo "=== Finished executing program : $1 ==="
