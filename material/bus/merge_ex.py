#!/usr/bin/env python2

# usage: merge.py "This is my stop" 10:34 gps_info.txt sched_info.txt > label_info.txt

import sys
import sched_ex

route_names = {
        3:   'Park Street',
        29:  'Central Park',
        848: 'Wall Street',
        6:   'Manhattan Square',
        72:  'Hill Street'}

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

def parse_data(now, files):
    ret = list()
    for fn in files:
        with open(fn, 'r') as f:
            for line in f:
                routenr, startnr, hr, minute, kind = [int(n) for n in line.split()]
                ret.append(BusInfo(routenr, startnr, hr, minute, kind))

    passed = set([(bi.routenr, bi.startnr) for bi in ret if bi.kind == Kind.Passed])
    have_gps_data = set([(bi.routenr, bi.startnr) for bi in ret if bi.kind == Kind.GPS])
    ret = [bi for bi in ret if (bi.routenr, bi.startnr) not in passed]
    ret = [bi for bi in ret if (bi.routenr, bi.startnr) not in have_gps_data or bi.kind == Kind.GPS]
    ret.sort(key=lambda bi: sched_ex.diff_time(now, (bi.hour, bi.minute)))
    return ret

def main():
    now = [int(n) for n in sys.argv[2].split(':')]
    files = sys.argv[3:5]
    data = parse_data(now, files)

    print sys.argv[1]
    print sys.argv[2]
    for bi in data[:7]:
        print bi.routenr
        print route_names[bi.routenr]
        if bi.kind == Kind.Schedule:
            print 'ca.',
        print '%02d:%02d' % (bi.hour, bi.minute)

if __name__ == '__main__':
    main()
