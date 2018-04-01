import math

route_names = {
        3:   'Park Street',
        29:  'Central Park',
        848: 'Wall Street',
        6:   'Manhattan Square',
        72:  'Hill Street'}

routes = [(3, (-4000.0, -4000.0)),
        (29,  (2000.0, 5000.0)),
        (848, (-4500.0, 5000.0)),
        (6,   (-3500.0, -5000.0)),
        (72,  (-4800.0, 4200.0))]

def dist(a, b):
    return math.sqrt(((a[0] - b[0]) ** 2) + ((a[1] - b[1]) ** 2))

class Time(object):
    def __init__(self, h, m):
        self.h = h
        self.m = m

# return how many minutes t2 is ahead of t1
def diff_time(t1, t2):
    hd = t2.h - t1.h
    md = t2.m - t1.m
    diff = hd * 60 + md
    # wrap around
    if diff < -720:
        diff += 1440
    return diff


