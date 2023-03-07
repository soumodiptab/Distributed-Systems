import sys

for line in sys.stdin:
    line = line.strip()
    line = line.split()
    if line[0] == 'a':
        # j -> (a, i, val)
        print("{}\t{} {} {}".format(line[2],line[0],line[1],line[3]))
    elif line[0] == 'b':
        # j -> (b, k, val)
        print("{}\t{} {} {}".format(line[1],line[0],line[2],line[3]))
        