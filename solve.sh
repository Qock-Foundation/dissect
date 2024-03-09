#!/bin/bash

LATTICE=$1
K=$2
./cmake-build-release/dissect $LATTICE $K | python3.11 visualize.py $LATTICE
