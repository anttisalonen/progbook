import math
import itertools

import random

random.seed(21)
N = 8

input_data = list()
for i in xrange(N):
    x = random.randint(0, 100)
    y = random.randint(0, 100)
    input_data.append((x, y))

def fac(n):
    return math.factorial(n)

for n in range(21):
    f = fac(n)
    print n, f, f * 4 / (1024 * 1024 * 1024.0)

def dist(a, b):
    a0, a1 = a
    b0, b1 = b
    d0 = b0 - a0
    d1 = b1 - a1
    return math.sqrt(d0 * d0 + d1 * d1)

def route_dist(route):
    total = 0
    for r1, r2 in zip(route, route[1:]):
        total += dist(r1, r2)
    total += dist(route[-1], route[0])
    return total

all_permutations = list(itertools.permutations(input_data))

min_dist = 100000
min_route = None
for p in all_permutations:
    dis = route_dist(p)
    if dis < min_dist:
        min_dist = dis
        min_route = list(p)
        min_route.append(p[0])

print min_dist, min_route

min_route_heuristic = [input_data[0]]
open_nodes = set(input_data)
open_nodes.remove(input_data[0])
current_node = input_data[0]
while open_nodes:
    min_dist = 100000
    min_point = None
    for p2 in input_data:
        if p2 not in open_nodes:
            continue
        dis = dist(current_node, p2)
        if dis < min_dist:
            min_dist = dis
            min_point = p2
    if min_point is not None:
        current_node = min_point
        open_nodes.remove(current_node)
        min_route_heuristic.append(current_node)
min_route_heuristic.append(input_data[0])

min_dist_heuristic = route_dist(min_route_heuristic)
print min_dist_heuristic, min_route_heuristic
assert(set(min_route) == set(min_route_heuristic))
