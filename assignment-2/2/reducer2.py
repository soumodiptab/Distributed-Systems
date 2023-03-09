import sys
rows = int(sys.argv[1])
cols = int(sys.argv[2])
#rows = 2
#cols = 2
# create an empty matrix of size 4x5
matrix = [[0 for x in range(cols)] for y in range(rows)]

for line in sys.stdin:
    line = line.strip()
    tokens = line.split('\t')
    i,k = tokens[1].split(',')
    val = tokens[2]
    matrix[int(i)][int(k)] += int(val)

# print the matrix
for i in range(rows):
    for j in range(cols):
        print("{} ".format(matrix[i][j]), end='')
    print()