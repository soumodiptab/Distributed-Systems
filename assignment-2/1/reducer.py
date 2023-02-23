import sys
current_state = None
zone_counter = 0
zone_votes = 0
zone_total = 0
zone_flag = 0
for line in sys.stdin:
    line = line.strip()
    state,rel = line.split("\t")
    zone_info = rel.split(" ")
    zone = zone_info[0]
    votes = int(zone_info[1])
    total = int(zone_info[2])
    flag = int(zone_info[3])
    if current_state == state:
        zone_counter += 1
        zone_votes += votes
        zone_total += total
        zone_flag += flag
    else:
        if current_state:
            print("%s\t%s\t%s\t%s\t%s" % (current_state, zone_counter, zone_votes, zone_total, zone_flag))
        current_state = state
        zone_counter = 1
        zone_votes = votes
        zone_total = total
        zone_flag = flag

if current_state == state:
    print("%s\t%s\t%s\t%s\t%s" % (current_state, zone_counter, zone_votes, zone_total, zone_flag))