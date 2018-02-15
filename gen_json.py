#!/usr/bin/env python2

import random
import json

l = list()

for i in xrange(10000):
    a = random.random() * 20 - 10.0
    b = random.random() * 20 - 10.0
    c = random.random() * 20 - 10.0
    l.append([a, b, c])

print json.dumps(l, indent=4)
