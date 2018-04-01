#!/usr/bin/env python2

# generate fake gps data
# input: time + gen_sched.py output + gen_gps.py output
# output: some gps samples in the following format:
# routenr startnr x y <flag already passed or not>

import sys
import random

import util
import sched

random.seed(21)

time = sys.argv[1]
filename = sys.argv[2]
datafilename = sys.argv[3]

# buses soon to approach the stop according to schedule
entries = sched.get_entries(time, filename, True)

# parse gps training data
data = list()
with open(datafilename, 'r') as f:
    for line in f:
        l = line.split(' ')
        routenr = int(l[0])
        t = float(l[1])
        px = float(l[2])
        py = float(l[3])
        data.append((routenr, t, px, py))

# generate gps entry for each planned bus that hasn't
# yet reached the stop
for bus_routenr, startnr, dt, _ in entries:
    options = list()
    # new_dt is when the bus is expected to actually arrive
    new_dt = dt + random.uniform(-2.0, 5.0)
    for routenr, t, px, py in data:
        # pick one of the data points from the historical data
        # pick one that's for this bus route and about the same
        # time as this one
        if bus_routenr != routenr:
            continue
        timediff = abs(t - new_dt)
        if timediff < 1.0:
            options.append((px, py))
    if options and random.random() < 0.9:
        ox, oy = random.choice(options)
        ox += random.uniform(-10.0, 10.0)
        oy += random.uniform(-10.0, 10.0)
        print bus_routenr, startnr, ox, oy, '2' if new_dt < 0.1 else '0', dt, new_dt

