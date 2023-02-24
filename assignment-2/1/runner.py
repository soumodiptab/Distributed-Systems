import sys
s = int(input())
p = float(input())
z = int(input())
with open("./input/f1.txt", "w") as f:
    for _ in range(1,z+1):
        line = input()
        if len(line.split(" ")) != 5:
            print("Invalid input")
            sys.exit()
        f.write(line+" "+str(p)+"\n")
    