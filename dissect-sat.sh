#!/bin/bash

LATTICE=$1
K=$2
DESTINATION=$3
./cmake-build-release/dissect-sat $LATTICE $K | python3 vis.py $LATTICE $DESTINATION
