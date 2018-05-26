#!/bin/bash

set -e
set -u

TIME="05:15"
SCHED_OUT=sched_out.txt
GPS_OUT=gps_out.txt
MERGE_OUT=merge_out.txt

./sched_ex.py $TIME sched_test.txt > $SCHED_OUT
./parse_gps_ex.py $TIME current_gps_ex.txt historical_gps_ex.txt > $GPS_OUT
./merge_ex.py "My stop" $TIME $GPS_OUT $SCHED_OUT > $MERGE_OUT
./bus $MERGE_OUT
