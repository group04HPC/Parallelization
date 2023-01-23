#!/bin/bash

# Course: High Performance Computing 2022/2023
#
# Lecturer: Francesco Moscato    fmoscato@unisa.it
#
# Group:
# Ferrara Grazia   0622701901  g.ferrara75@studenti.unisa.it
# Franco Paolo     0622701993  p.franco9@studenti.unisa.it
#
# Copyright (C) 2023 - All Rights Reserved
#
# This file is part of Project Assignment 2022/2023.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Project Assignment 2022/2023.  If not, see http://www.gnu.org/licenses/.
#
# Requirements of the assignment:
# Provide a parallell version of the Tarjan's algorithm to find Strongly Connected Components in a Graph.
# The implementation MUST use a message passing paradigm, and has to be implemented by using MPI.
# Students MUST store and load the input graph FROM FILES. The whole graph MUST be distributed on files
# on each node (i.e.: the whole graph cannot be stored on a single (even replicated) file). Good Graph
# dimensions are greater than 4GB of data. Students have to choose the proper data structure to
# represent the graph in memory.
#
# Purpose of the file:
# This file contains the script used to try some test cases 

ARRAY_SZ=(400 800 1200 1600 2000 2400)
ARRAY_PROC=(1 2 4 8)
ARRAY_SHRINK=(1 2 4)

for graphSize in "${ARRAY_SZ[@]}"; do
    for proc in "${ARRAY_PROC[@]}"; do
        for shrink in "${ARRAY_SHRINK[@]}"; do
            echo -e "\t"Testing executed on a $graphSize vertices graph
            echo -e "\t"Testing executed with $proc processes
            echo -e "\t"Testing executed with $shrink shrink factor
            wl=$(($graphSize/$proc))
            min=$(($graphSize/$shrink))
            min=$min-10
            max=$(($graphSize/$shrink))
            ./Source/updateConstants.sh $wl $min $max
            ./run.sh $proc
        done
    done
done
