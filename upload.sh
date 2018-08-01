#!/bin/bash

set -e
set -u

lftp -e "mirror -R _build/html /" -u $USERNAME,$PASSWORD $HOST
