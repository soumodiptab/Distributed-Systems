#!/bin/bash
# docker exec -it namenode bash -c "apt update && apt install python3 -y"
# docker exec -it datanode bash -c "apt update && apt install python3 -y"
# docker exec -it resourcemanager bash -c "apt update && apt install python3 -y"
# docker exec -it nodemanager bash -c "apt update && apt install python3 -y"
# docker exec -it historyserver bash -c "apt update && apt install python3 -y"
cd $1
echo "Removing old input"
docker exec -it namenode bash -c "rm -r input"
echo "Copying files to namenode..."
docker cp mapper.py namenode:mapper.py
docker cp reducer.py namenode:reducer.py
docker cp input namenode:input
docker cp start.sh namenode:start.sh
echo "All files copied to namenode."
echo "Running start.sh on namenode..."
docker exec -it namenode bash -c "./start.sh"
echo "MapReduce job completed."
echo "====================  OUTPUT  ===================="
docker exec -it namenode bash -c "hdfs dfs -cat output/*"
echo "=================================================="
cd ..
