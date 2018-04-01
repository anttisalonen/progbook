#!/usr/bin/env python2

import sys
import math
import random
import json
import util

random.seed(21)

def progress(pos, speed):
    length = random.uniform(40.0, 110.0)
    length = length * speed
    xcomp = abs(pos[0]) / abs(pos[1])
    ycomp = 1.0 / xcomp
    return (pos[0] + xcomp * length * -math.copysign(1.0, pos[0]),
            pos[1] + ycomp * length * -math.copysign(1.0, pos[1]))

out = list()
for route, start_pos in util.routes:
    for n in xrange(100):
        this = list()
        curr_pos = (start_pos[0] + random.uniform(-5.0, 5.0),
                    start_pos[1] + random.uniform(-5.0, 5.0))
        curr_time = 0
        positions = list()
        speed = random.uniform(0.5, 1.0)
        while util.dist(curr_pos, (0, 0)) > 100.0:
            positions.append(curr_pos)
            curr_pos = progress(curr_pos, speed)
        final_positions = list()
        time_skew = random.uniform(0.0, 10.0)
        for i, p in enumerate(positions):
            this.append({'route': route,
                'time': ((len(positions) - i) * 10.0 + time_skew + random.uniform(0.0, 0.1)) / 60.0,
                'pos': p})
        out.append(this)

#print json.dumps(out, indent=True)
for el in out:
    for i in el:
        print i['route'], i['time'], i['pos'][0], i['pos'][1]
