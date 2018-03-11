#!/bin/bash

./gen_dot_labels.sh > dep_labels.dot
cat dep_header.dot dep_labels.dot dep_footer.dot > dep.dot

./gen_dot2.py > dep2.dot
