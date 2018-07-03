import math

a = 3.0
b = -2.0
c = -3.0
det = b * b - 4.0 * a * c
if det < 0:
    num_roots = 0
r1 = (-b + math.sqrt(det)) / (2.0 * a)
if det == 0:
    num_roots = 1
else:
    num_roots = 2
r2 = (-b - math.sqrt(det)) / (2.0 * a)
print '%d roots' % num_roots
if num_roots == 1:
    print 'Root: %.2f' % r1
elif num_roots == 2:
    print "Root 1: %.2f - Root 2: %.2f\n" % (r1, r2)
