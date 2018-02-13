with open('test.txt', 'w') as f:
    for i in xrange(5):
        f.write("%f %f\n" % (0.2, 0.5))
