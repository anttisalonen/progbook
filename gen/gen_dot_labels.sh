#!/bin/bash

for n in rst/*.rst; do
    if [[ $n != *"index"* ]]; then
        if [[ $n != "rst/ex_"* && $n != "rst/dep.rst" ]]; then
            label=$(head -n1 $n)
            echo "    $(basename $n .rst) [label=\"$label\"];"
        fi
    fi
done
