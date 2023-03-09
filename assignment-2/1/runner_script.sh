#!/bin/bash
#echo "Updating Python..."
#apt update && apt install python3 -y
# $1 = path to jar file $2 = path to input file $3 hdfs input dir $4 hdfs output dir $5 = mapreduce file loc

JAR_FILE=$1
INPUT_FILE=$2
HDFS_INPUT_DIR=$3
HDFS_OUTPUT_DIR=$4
MAPREDUCE_DIR=$5


cd $MAPREDUCE_DIR
rm -r output
rm -r input
mkdir input
mkdir output
echo "=== Running input formatting script... ==="
cat $INPUT_FILE | python3 input_formatter.py
if [ $? != 0 ]; then
    echo "Incorrect input format."
    exit 1
fi
echo "=== Generating input files for mapreduce job... ==="
ls -l ./input
echo "=== removing old output and input from HDFS... ==="
hdfs dfs -rm -r $HDFS_OUTPUT_DIR
hdfs dfs -rm -r $HDFS_INPUT_DIR
echo "=== Creating input directory... ==="
hadoop fs -mkdir -p $HDFS_INPUT_DIR
echo "=== Copying input files to HDFS... =                     =="
hdfs dfs -put input/* $HDFS_INPUT_DIR
echo "=== Running MapReduce JOB1 ==="
# alternate command
hadoop jar $JAR_FILE -files mapper.py,reducer.py  -mapper 'python3 mapper.py' -reducer 'python3 reducer.py' -input $HDFS_INPUT_DIR -output $HDFS_OUTPUT_DIR
#mapred streaming -files mapper.py,reducer.py -mapper 'python3 mapper.py' -reducer 'python3 reducer.py' -input $HDFS_INPUT_DIR -output $HDFS_OUTPUT_DIR
echo "=== MapReduce job completed. ==="
rm -r input
mkdir input
echo "=== Preparing for JOB2, removing old input files ==="
hdfs dfs -cat $HDFS_OUTPUT_DIR/* >input/f1.txt
echo "====================  STAGE 1 OUPUT  ===================="
cat input/f1.txt
echo "========================================================="
echo "=== removing old output and input from HDFS... ==="
hdfs dfs -rm -r $HDFS_OUTPUT_DIR
hdfs dfs -rm -r $HDFS_INPUT_DIR
echo "=== Creating input directory... ==="
hadoop fs -mkdir -p $HDFS_INPUT_DIR
echo "=== Copying input files to HDFS... ==="
hdfs dfs -put input/* $HDFS_INPUT_DIR
echo "=== Running MapReduce JOB2 ==="
hadoop jar $JAR_FILE -files mapper2.py,reducer2.py  -mapper 'python3 mapper2.py' -reducer 'python3 reducer2.py' -input $HDFS_INPUT_DIR -output $HDFS_OUTPUT_DIR
#mapred streaming -files mapper2.py,reducer2.py -mapper 'python3 mapper2.py' -reducer 'python3 reducer2.py' -input input -output output
echo "=== MapReduce job completed. ==="
hdfs dfs -cat $HDFS_OUTPUT_DIR/* >output/out.txt
echo "====================  STAGE 2 OUPUT  ===================="
cat output/out.txt
echo "========================================================="
cd ..
