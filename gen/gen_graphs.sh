#!/bin/bash

python2 material/dot/dot2.py < gen/dep.dot  | dot -Tpng > gen/dependencies.png
python2 material/dot/dot2.py < gen/dep2.dot | dot -Tpng > gen/dependencies2.png

