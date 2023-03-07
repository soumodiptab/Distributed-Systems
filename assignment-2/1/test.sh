#!/bin/bash
rm -r output
rm -r input
mkdir input
mkdir output
cat inp.txt | python3 input_formatter.py
cat input/f1.txt | python3 mapper.py | sort -k1,1 | python3 reducer.py | python3 mapper2.py | sort -k1,1 | python3 reducer2.py | sort -k1,1 >output/out.txt
