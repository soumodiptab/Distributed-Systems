#!/bin/bash
cat inp.txt | python3 input_formatter.py
source variables
cat input/mat.txt | python3 mapper.py | sort -k1,1 | python3 reducer.py | python3 mapper2.py | sort -k1,1 | python3 reducer2.py $ROWS $COLS
echo "=== FINAL DIMENSIONS ==="
echo "ROWS: $ROWS"
echo "COLS: $COLS"

"python3 reducer2.py $ROWS $COLS"
