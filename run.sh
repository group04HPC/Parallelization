# !/bin/bash

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
# This file contains the script used to customize the execution of both the serial and parallel programs

# Check if the user asked for help
if [ "$1" = "-h" ]; then
	echo -e "Executes the program on a given number of processes. If the [run serial] parameter is not inserted it will execute it as default\n"
	echo -e "Usage:\t$0 [num of processes]"
	echo -e "\t$0 [num of processes] [run serial (0/1)]"
	echo -e "\t$0 -h\t-to print this help output"
	echo -e "\t$0 -t\t-to execute a series of tests"
	exit 0
fi

# Run various tests
if [ "$1" = "-t" ]; then
	echo "Running a test on 4 processes"
	./test.sh 4

	echo "Running a test on 8 processes"
	./test.sh 4

	echo "Running a test on 16 processes"
	./test.sh 4
	exit 0
fi
# Check if the number of arguments is correct
if [ $# -le 0 ]; then
	echo "You have to specify the number of processes!\n Try using using \"$0 -h\" for more info"
	exit 1
fi

# Check if the script is called with 1 param 
if [ $# -eq 1 ]; then
	serial=1
fi

#Check if the script is called with 2 param or more
if [ $# -ge 2 ]; then
	serial=$2
fi

# Each process creates its own binary file with a part of the full graph

mpirun -np $1 ./Build/wg.o

if [ "$serial" -ne 0 ]; then
	# This program reads all the binary files and creates a single graph, writing it into a text file
	# so that the serial version can be used to compare the results

	mpirun -np $1 ./Build/rg.o

	# The serial program executes and writes its result on a file
	./Build/s.o
fi

# The parallel program executes and writes its result on a file
mpirun -np $1 ./Build/p.o

if [ "$serial" -ne 0 ]; then
	# The results of both the serial and the parallel version are compared  
	./Build/c.o
fi
