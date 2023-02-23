import sys
for line in sys.stdin:
    line = line.strip()
    zone_info = line.split(" ")
    state = zone_info[1]
    zone =  zone_info[0]
    votes = zone_info[2]
    total = zone_info[3]
    if zone_info[4] == "YES":
        flag = 1
    else:
        flag = 0
    print(state + "\t" + zone + " "+ votes + " " + total + " " + str(flag))
