#!/usr/bin/env python2

# generate a higher level chapter depedency graph

# first read in the lower level dependencies
# then read in the chapter descriptions from the index rst files
# then find the dependencies between higher level chapters

import re
import collections
import glob
import sys

def get_low_deps():
    reg = re.compile(" *(\S+) *-> *(\S+);")
    ret1 = collections.defaultdict(list)
    ret2 = collections.defaultdict(list)
    with open('dep.dot', 'r') as f:
        for line in f:
            res = reg.match(line)
            if res:
                ret1[res.group(1)].append(res.group(2))
                ret2[res.group(2)].append(res.group(1))
    return ret1, ret2

def get_chap_desc():
    files = glob.glob("*_index.rst")
    reg = re.compile(" *([a-z0-9_]+)$")
    ret1 = collections.defaultdict(list)
    ret2 = dict()
    ret3 = dict()
    for fn in files:
        if fn.startswith('ex_'):
            continue
        fn_clean = fn.strip('.rst')
        have_header = False
        with open(fn, 'r') as f:
            for line in f:
                if not have_header:
                    ret3[fn_clean] = line.strip()
                    have_header = True
                res = reg.match(line)
                if res:
                    ret1[fn_clean].append(res.group(1))
                    ret2[res.group(1)] = fn_clean
    return ret1, ret2, ret3

def get_deps(low_deps, rev_low_deps, chap_desc, rev_chap_desc):
    deps = collections.defaultdict(set)
    for ch, ch_cont in chap_desc.items():
        for subch in ch_cont:
            low_deps = rev_low_deps[subch]
            for ld in low_deps:
                try:
                    hd = rev_chap_desc[ld]
                except KeyError:
                    print >> sys.stderr, 'Warning: chapter "%s" referenced in dependencies but not written' % ld
                else:
                    deps[ch].add(hd)
    return deps

def serialise(deps, headers):
    print 'digraph dep2 {'
    for chap, header in sorted(headers.items()):
        print '    %s [label="%s"];' % (chap, header)
    print
    for chap, to in sorted(deps.items()):
        for i in to:
            if i != chap:
                print '    %s -> %s;' % (i, chap)
    print '}'

def main():
    low_deps, rev_low_deps = get_low_deps()
    chap_desc, rev_chap_desc, headers = get_chap_desc()
    deps = get_deps(low_deps, rev_low_deps, chap_desc, rev_chap_desc)
    serialise(deps, headers)

if __name__ == "__main__":
    main()
