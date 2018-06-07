#!/usr/bin/env python2

import re
import collections

reg = re.compile(' *([a-zA-Z0-9_]+) *-> *([a-zA-Z0-9_]+)')

dep = collections.defaultdict(list)

# read in the input file; output lines that are not connections
with open('/dev/stdin', 'r') as f:
    for line in f:
        res = re.match(reg, line)
        if res:
            dep[res.group(1)].append(res.group(2))
        else:
            if '}' not in line:
                print line,

# return true if to_node is a child of from_node
def is_reachable(dep, from_node, to_node):
    for child in dep[from_node]:
        if child == to_node:
            return True
        if is_reachable(dep, child, to_node):
            return True
    return False

# remove redundant edges
# an edge from a to b is redundant if a connection from a child of a to b is found
for parent_node, my_edges in dep.items():
    for direct_child in list(my_edges):
        for other_child in list(my_edges):
            if is_reachable(dep, direct_child, other_child):
                dep[parent_node].remove(other_child)

# output
for a, deps in dep.items():
    for b in deps:
        print '    %s -> %s;' % (a, b)

print '}'
