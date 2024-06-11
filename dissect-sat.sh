#!/bin/bash

LATTICE=$1
FIGURE=$2
K=$3
DESTINATION=$4
./cmake-build-release/dissect-sat $LATTICE $FIGURE $K | python3 vis.py $LATTICE $DESTINATION
