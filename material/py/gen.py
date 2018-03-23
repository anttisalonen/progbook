import random

with open('test.txt', 'w') as f:
    for n in xrange(10000):
        a = random.random() * 20 - 10.0
        b = random.random() * 20 - 10.0
        c = random.random() * 20 - 10.0
        f.write('%f %f %f\n' % (a, b, c))
