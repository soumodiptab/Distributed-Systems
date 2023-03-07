import sys
a_rows , a_cols = [int(x) for x in input().split()]
a=[]
# take matrix input from command line
for i in range(a_rows):
    a.append([int(x) for x in input().split()])
b_rows , b_cols = [int(x) for x in input().split()]
b=[]
# take matrix input from command line
for i in range(b_rows):
    b.append([int(x) for x in input().split()])

# check if the matrix dimensions are valid
if a_cols != b_rows:
    print("Invalid input")
    exit(1)

# parse the matrix a in a file in sparse format
with open('./input/mat.txt', 'w') as f:
    for i in range(a_rows):
        for j in range(a_cols):
            if a[i][j] != 0:
                f.write('a '+ str(i) + ' ' + str(j) + ' ' + str(a[i][j]) + '\n')

with open('./input/mat.txt', 'a') as f:
    for i in range(b_rows):
        for j in range(b_cols):
            if b[i][j] != 0:
                f.write('b '+ str(i) + ' ' + str(j) + ' ' + str(b[i][j]) + '\n')

with open('./variables','w') as f:
    f.write('ROWS="{}"\n'.format(a_rows))
    f.write('COLS="{}"\n'.format(b_cols))