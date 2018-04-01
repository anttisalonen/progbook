#!/bin/bash

set -e
set -u

# fake data - gps historical data and schedule
gen_gps.py > gps.txt
gen_sched.py > sched.txt

# fake data - current gps data
gps.py 10:00 sched.txt gps.txt > gps_raw.txt

# display
bus "This is my C++" gps_raw.txt gps.txt sched.txt 10:00

