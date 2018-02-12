#!/bin/bash

for n in *.rst; do
    if [[ $n != *"index"* ]]; then
        if [[ $n != "ex_"* && $n != "dep.rst" ]]; then
            label=$(head -n1 $n)
            echo "    $(basename $n .rst) [label=\"$label\"];"
        fi
    fi
done
