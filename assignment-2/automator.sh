#!/bin/bash
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
docker exec -it namenode bash -c "./$1/start.sh $1"
cd ..
echo "=== Finished executing program : $1 ==="
