#!/bin/bash
python3 input_formatter.py
source variables
cat input/mat.txt | python3 mapper.py | sort -k1,1 | python3 reducer.py | python3 mapper2.py | sort -k1,1 | python3 reducer2.py $ROWS $COLS
