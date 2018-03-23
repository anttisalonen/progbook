#!/bin/bash

gen/gen_dot_labels.sh > gen/dep_labels.dot
cat gen/dep_header.dot gen/dep_labels.dot gen/dep_footer.dot > gen/dep.dot

gen/gen_dot2.py > gen/dep2.dot
