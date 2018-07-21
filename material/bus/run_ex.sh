#!/bin/bash

set -e
set -u

TIME="05:15"
SCHED_OUT=sched_out.txt
GPS_OUT=gps_out.txt
MERGE_OUT=merge_out.txt

./sched.py $TIME sched_test.txt > $SCHED_OUT
./parse_gps.py $TIME current_gps.txt historical_gps.txt > $GPS_OUT
./merge.py "My stop" $TIME $GPS_OUT $SCHED_OUT > $MERGE_OUT
./bus $MERGE_OUT
