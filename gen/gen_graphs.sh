#!/bin/bash

dot -Tpng gen/dep.dot > gen/dependencies.png
dot -Tpng gen/dep2.dot > gen/dependencies2.png

