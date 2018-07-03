import math
import sys

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

max_root = -1
max_a = -1
max_b = -1
max_c = -1
with open('test.txt', 'r') as f:
    for line in f:
        words = line.split()
        numbers = list()
        for word in words:
            numbers.append(float(word))
        if len(numbers) != 3:
            print >> sys.stderr, 'Could not parse'
        else:
            roots = quad(numbers[0], numbers[1], numbers[2])
            for root in roots:
                if root > max_root:
                    max_root = root
                    max_a = numbers[0]
                    max_b = numbers[1]
                    max_c = numbers[2]

    print 'Maximum root is %.2f for equation (%.3f, %.3f, %.3f)' % (max_root, max_a, max_b, max_c)

