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
# This file contains the script used to test the program on the lecturer's cluster

echo Comparing performances when changing number of edges
graphSize=12000
echo -e "\t"Testing executed on a $graphSize vertices graph and 4 processes

echo 0-2000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 0 2000
./run.sh 4

echo 2000-4000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 2000 4000
./run.sh 4

echo 4000-6000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 4000 6000
./run.sh 4

echo 6000-8000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 6000 8000
./run.sh 4

echo 8000-10000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 8000 10000
./run.sh 4

echo 10000-12000 edges
./Source/updateConstants.sh $(($graphSize / 4)) 10000 12000
./run.sh 4

echo Comparing performances when changing graph size and number of processes

echo Case 3 - 16 processes
./Source/updateConstants.sh 250 0 250
./run.sh 16

echo Case 3 - 8 processes
./Source/updateConstants.sh 500 0 500
./run.sh 8 0

echo Case 3 - 4 processes
./Source/updateConstants.sh 1000 0 1000
./run.sh 4 0

echo Case 3 - 2 processes
./Source/updateConstants.sh 2000 0 2000
./run.sh 2 0

echo Case 3 - 1 process
./Source/updateConstants.sh 4000 0 4000
./run.sh 1 0

echo Case 2 - 16 processes
./Source/updateConstants.sh 1000 0 1000
./run.sh 16

echo Case 2 - 8 processes
./Source/updateConstants.sh 2000 0 2000
./run.sh 8 0

echo Case 2 - 4 processes
./Source/updateConstants.sh 4000 0 4000
./run.sh 4 0

echo Case 2 - 2 processes
./Source/updateConstants.sh 8000 0 8000
./run.sh 2 0

echo Case 2 - 1 process
./Source/updateConstants.sh 16000 0 16000
./run.sh 1 0

echo Case 1 - 16 processes
./Source/updateConstants.sh 1250 0 1250
./run.sh 16 

echo Case 1 - 8 processes
./Source/updateConstants.sh 2500 0 2500
./run.sh 8 0

echo Case 1 - 4 processes
./Source/updateConstants.sh 5000 0 5000
./run.sh 4 0

echo Case 1 - 2 processes
./Source/updateConstants.sh 10000 0 10000
./run.sh 2 0

echo Case 1 - 1 process
./Source/updateConstants.sh 20000 0 20000
./run.sh 1 0

echo Case 0 - 16 processes
./Source/updateConstants.sh 1500 0 1500
./run.sh 16 

echo Case 0 - 8 processes
./Source/updateConstants.sh 3000 0 3000
./run.sh 8 0

echo Case 0 - 4 processes
./Source/updateConstants.sh 6000 0 6000
./run.sh 4 0

echo Case 0 - 2 processes
./Source/updateConstants.sh 12000 0 12000
./run.sh 2 0

echo Case 0 - 1 process
./Source/updateConstants.sh 24000 0 24000
./run.sh 1 0

# Da completare
echo Case -1 - 16 processes
./Source/updateConstants.sh 1500 0 1500
./run.sh 16 

echo Case -1 - 8 processes
./Source/updateConstants.sh 3000 0 3000
./run.sh 8 0

echo Case -1 - 4 processes
./Source/updateConstants.sh 6000 0 6000
./run.sh 4 0

echo Case -1 - 2 processes
./Source/updateConstants.sh 12000 0 12000
./run.sh 2 0

echo Case -1 - 1 process
./Source/updateConstants.sh 24000 0 24000
./run.sh 1 0