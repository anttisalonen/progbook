import math

def quad(a, b, c):
    det = b * b - 4.0 * a * c
    if det < 0:
        return []
    r1 = (-b + math.sqrt(det)) / (2.0 * a)
    if det == 0:
        return [r1]
    else:
        r2 = (-b - math.sqrt(det)) / (2.0 * a)
        return [r1, r2]

a = 3.0
b = -2.0
c = -3.0

res = quad(a, b, c)

print '%d roots' % len(res)
for r in res:
    print 'Root: %.2f' % r
