#!/bin/bash

set -e
set -u

# fake data - gps historical data and schedule
gen_gps.py > gps.txt
gen_sched.py > sched.txt

# fake data - current gps data
now=$(date +"%H:%M")
gps.py $now sched.txt gps.txt > gps_raw.txt

bus "This is my C++" gps_raw.txt gps.txt sched.txt &

while [ 1 ]; do
    sleep 10

    # fake data - current gps data
    now=$(date +"%H:%M")
    gps.py $now sched.txt gps.txt > gps_raw.txt
done

