#!/bin/bash

tred < gen/dep.dot  | dot -Tpng > gen/dependencies.png
tred < gen/dep2.dot | dot -Tpng > gen/dependencies2.png

