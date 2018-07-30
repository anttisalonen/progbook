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
    with open('gen/dep.dot', 'r') as f:
        for line in f:
            res = reg.match(line)
            if res:
                ret1[res.group(1)].append(res.group(2))
                ret2[res.group(2)].append(res.group(1))
    return ret1, ret2

def parse_stage(fn):
    chapters = list()
    have_header = False
    with open(fn, 'r') as f:
        for line in f:
            if not have_header:
                title = line.strip()
                have_header = True
            else:
                if line.strip().endswith('_index'):
                    chapters.append(line.strip())
    return title, chapters

def get_chap_desc():
    files = glob.glob("rst/*_index.rst")
    reg = re.compile(" *([a-z0-9_]+)$")
    ret1 = collections.defaultdict(list)
    ret2 = dict()
    ret3 = dict()
    stages = dict()
    for fn in files:
        if fn.startswith('rst/ex_'):
            continue
        if fn.startswith('rst/ch'):
            stage = int(fn[6])
            stages[stage] = parse_stage(fn)
            continue
        fn_clean = fn.replace('rst/', '').replace('.rst', '')
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
    return ret1, ret2, ret3, stages

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

def serialise(deps, headers, stages):
    print 'digraph dep2 {'
    print '    compound=true;'
    for stagenum, (title, chapters) in sorted(stages.items()):
        print '    subgraph cluster%d {' % (stagenum - 1)
        for chap in sorted(chapters):
            print '        %s [label="%s"];' % (chap, headers[chap])
        print '    labelloc="t"';
        print '    label="%s"' % title;
        print '    }'
    print
    for chap, to in sorted(deps.items()):
        for i in to:
            if i != chap:
                print '    %s -> %s;' % (i, chap)
    print '}'

def main():
    low_deps, rev_low_deps = get_low_deps()
    chap_desc, rev_chap_desc, headers, stages = get_chap_desc()
    deps = get_deps(low_deps, rev_low_deps, chap_desc, rev_chap_desc)
    serialise(deps, headers, stages)

if __name__ == "__main__":
    main()
