#!/bin/bash

LATTICE=$1
K=$2
DESTINATION=$3
./cmake-build-release/dissect $LATTICE $K | python3 vis.py $LATTICE $DESTINATION
