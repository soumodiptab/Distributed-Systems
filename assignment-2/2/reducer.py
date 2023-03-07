import sys
mat_a=[]
mat_b=[]
for line in sys.stdin:
    tokens = line.strip()
    tokens = tokens.split('\t')
    j = tokens[0]
    tokens = tokens[1].split()
    if tokens[0] == 'a':
        # j,i
        mat_a.append((j, tokens[1], tokens[2]))
    elif tokens[0] == 'b':
        # j,k
        mat_b.append((j, tokens[1], tokens[2]))

for a in mat_a:
    for b in mat_b:
        if a[0] == b[0]:
            print("{},{}\t{}".format(a[1], b[1], int(a[2]) * int(b[2])))
