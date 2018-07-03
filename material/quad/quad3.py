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

def run(a, b, c):
    res = quad(a, b, c)
    print '%d roots' % len(res)
    for r in res:
        print 'Root: %.2f' % r

with open('test1.txt', 'r') as f:
    text = f.read()
    words = text.split()
    numbers = list()
    for word in words:
        numbers.append(float(word))
    if len(numbers) != 3:
        print >> sys.stderr, 'Could not parse'
    else:
        run(numbers[0], numbers[1], numbers[2])

