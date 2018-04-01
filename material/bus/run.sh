#!/bin/bash

set -e
set -u

# fake data - gps historical data and schedule
gen_gps.py > gps.txt
gen_sched.py > sched.txt

# display
bus label_info.txt &

while [ 1 ]; do
    # fake data - current gps data
    now=$(date +"%H:%M")
    gps.py $now sched.txt gps.txt > gps_raw.txt

    # get gps and schedule info
    parse_gps.py $now gps_raw.txt gps.txt > gps_info.txt
    sched.py $now sched.txt > sched_info.txt

    # merge gps and schedule info
    merge.py "This is my stop" $now gps_info.txt sched_info.txt > label_info.txt
    sleep 10
done
