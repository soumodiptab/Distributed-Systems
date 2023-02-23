#!/bin/bash
cat inp.txt | python3 mapper.py | sort -k1,1 | python3 reducer.py