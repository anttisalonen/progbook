#!/usr/bin/env python2

import sys
import math
import random
import json

random.seed(21)

for route in [3, 29, 848, 6, 72]:
    start_nr = 0
    occurrence = random.choice([10, 12, 15, 20])
    start_min = random.randint(0, occurrence - 1)
    for hr in xrange(5, 24):
        for minute in xrange(start_min, 60, occurrence):
            start_nr += 1
            print '%d %d %d %d' % (route, start_nr, hr, minute)

