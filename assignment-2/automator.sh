#!/bin/bash
# docker exec -it namenode bash -c "apt update && apt install python3 -y"
# docker exec -it datanode bash -c "apt update && apt install python3 -y"
# docker exec -it resourcemanager bash -c "apt update && apt install python3 -y"
# docker exec -it nodemanager bash -c "apt update && apt install python3 -y"
# docker exec -it historyserver bash -c "apt update && apt install python3 -y"
cd $1
echo "Removing old input"
docker exec -it namenode bash -c "rm -r $1"
echo "Copying files to namenode..."
# docker cp mapper.py namenode:mapper.py
# docker cp reducer.py namenode:reducer.py
# docker cp input namenode:input
# docker cp start.sh namenode:start.sh
docker cp $1 namenode:$1
echo "All files copied to namenode."
echo "Running start.sh on namenode..."
docker exec -it namenode bash -c "./$1/start.sh"
echo "====================  OUTPUT  ===================="
docker exec -it namenode bash -c "hdfs dfs -cat output/*"
echo "=================================================="
cd ..
