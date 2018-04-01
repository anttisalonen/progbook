#!/bin/bash

set -e
set -u

# fake data - gps historical data and schedule
gen_gps.py > gps.txt
gen_sched.py > sched.txt

# fake data - current gps data
gps.py 10:00 sched.txt gps.txt > gps_raw.txt

# get gps and schedule info
parse_gps.py 10:00 gps_raw.txt gps.txt > gps_info.txt
sched.py 10:00 sched.txt > sched_info.txt

# merge gps and schedule info
merge.py "Park Avenue" 10:00 gps_info.txt sched_info.txt > label_info.txt

# display
bus label_info.txt
