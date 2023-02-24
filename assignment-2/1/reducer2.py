import sys
from math import floor
state_counter = 0
seat_counter = 0
poll_counter = 0
total_seats = 0
national_votes = 0
national_total = 0
recog_counter = 0
NATIONAL=False
for line in sys.stdin:
    line = line.strip()
    state_info = line.split("\t")[1].split(" ")
    state_counter += 1
    zones = int(state_info[1])
    state_votes = int(state_info[2])
    state_total = int(state_info[3])
    state_flag = int(state_info[4])
    perc = float(state_info[5])
    if zones == state_flag:
        recog_counter += 1
    state_perc =float(state_votes)/float(state_total)*100
    if state_perc >= 6:
        poll_counter += 1
    seats = floor(state_perc/perc)
    if seats != 0:
        seat_counter += 1
        total_seats += seats
    national_votes += state_votes
    national_total += state_total
if seat_counter >= 3 and total_seats >= 3:
    NATIONAL = True
elif  poll_counter >= 4 and total_seats >= 4:
    NATIONAL = True
elif recog_counter >= 4:
    NATIONAL = True
print(NATIONAL)