#!/usr/bin/env python2

import sys
import util

def get_entries(time, filename, get_nearby):
    now_hr, now_minute = time.split(':')
    now = util.Time(int(now_hr), int(now_minute))

    entries = list()
    with open(filename, 'r') as f:
        for line in f:
            route, startnr, hr, mn = [int(n) for n in line.split(' ')]
            time = util.Time(hr, mn)
            dt = util.diff_time(now, time)
            if (get_nearby and abs(dt) < 50) or (not get_nearby and dt > 0):
                entries.append((route, startnr, dt, time))

    if get_nearby:
        entries.sort(key=lambda (r, s, dt, t): abs(dt))
    else:
        entries.sort(key=lambda (r, s, dt, t): dt)

    num_entries = 50
    return entries[:num_entries]

def main():
    time = sys.argv[1]
    filename = sys.argv[2]
    get_nearby = len(sys.argv) > 3 and sys.argv[3] == 'nearby'

    entries = get_entries(time, filename, get_nearby)
    for (r, s, dt, t) in entries:
        print r, s, t.h, t.m, 1

if __name__ == '__main__':
    main()
