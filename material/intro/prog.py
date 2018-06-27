for i in range(1, 11):
    for j in range(1, 11):
        print("%d * %d = %d" % (i, j, i * j))

def area(radius):
    return 3.14 * radius * radius

for i in range(1, 11):
    print("radius %d => area %f" % (i, area(i)))

def square(x):
    return x * x

for i in range(1, 11):
    a = area(i)
    if a > 10 and a < 100:
        print("%d %d" % (i, a))

for i in range(1, 11):
    a = area(i)
    s = square(i)
    print(a - s)

for i in range(1, 11):
    m = i % 3
    if m == 1:
        print(i)

for i in range(1, 101):
    if i % 15 == 0:
        print('FizzBuzz')
    elif i % 3 == 0:
        print('Fizz')
    elif i % 5 == 0:
        print('Buzz')
    else:
        print(i)

