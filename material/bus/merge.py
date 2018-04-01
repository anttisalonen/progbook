#!/usr/bin/env python2

# usage: merge.py "This is my stop" 10:34 gps_info.txt sched_info.txt > label_info.txt

import sys
import util

class Kind(object):
    GPS = 0
    Schedule = 1
    Passed = 2

def int_to_kind(n):
    for i in [Kind.Schedule, Kind.GPS, Kind.Passed]:
        if n == i:
            return n
    raise RuntimeError('No Kind')

class BusInfo(object):
    def __init__(self, routenr, startnr, hour, minute, kind):
        self.routenr = routenr
        self.startnr = startnr
        self.hour = hour
        self.minute = minute
        self.kind = int_to_kind(kind)

data = list()

for fn in sys.argv[3:5]:
    with open(fn, 'r') as f:
        for line in f:
            routenr, startnr, hr, minute, kind = [int(n) for n in line.split()]
            data.append(BusInfo(routenr, startnr, hr, minute, kind))

now_h, now_m = [int(n) for n in sys.argv[2].split(':')]
now = util.Time(now_h, now_m)
passed = set([(bi.routenr, bi.startnr) for bi in data if bi.kind == Kind.Passed])
have_gps_data = set([(bi.routenr, bi.startnr) for bi in data if bi.kind == Kind.GPS])
data = [bi for bi in data if (bi.routenr, bi.startnr) not in passed]
data = [bi for bi in data if (bi.routenr, bi.startnr) not in have_gps_data or bi.kind == Kind.GPS]
data.sort(key=lambda bi: util.diff_time(now, util.Time(bi.hour, bi.minute)))

print sys.argv[1]
print sys.argv[2]

for bi in data[:7]:
    print bi.routenr
    print util.route_names[bi.routenr]
    if bi.kind == Kind.Schedule:
        print 'ca.',
    print '%02d:%02d' % (bi.hour, bi.minute)
