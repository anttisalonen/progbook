#!/usr/bin/env python2

import sys

# return how many minutes t2 is ahead of t1
def diff_time((t1h, t1m), (t2h, t2m)):
    hd = t2h - t1h
    md = t2m - t1m
    diff = hd * 60 + md
    # wrap around
    if diff < -720:
        diff += 1440
    return diff

def get_entries(time, filename):
    now_hr, now_minute = [int(n) for n in time.split(':')]

    entries = list()
    with open(filename, 'r') as f:
        for line in f:
            route, startnr, hr, mn = [int(n) for n in line.split(' ')]
            dt = diff_time((now_hr, now_minute), (hr, mn))
            if dt > 0:
                entries.append((route, startnr, dt, hr, mn))

    entries.sort(key=lambda (r, s, dt, hr, mn): dt)

    return entries[:50]

def main():
    time = sys.argv[1]
    filename = sys.argv[2]

    entries = get_entries(time, filename)
    for (r, s, dt, th, tm) in entries:
        print r, s, th, tm, 1

    test()

def test():
    assert diff_time((6, 34), (6, 40)) == 6
    assert diff_time((6, 40), (6, 34)) != 6

if __name__ == '__main__':
    main()
