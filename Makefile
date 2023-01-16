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
# This file is the Makefile for the program, capable of compiling, when necessary, all the source files to make the executuion coherent

.SILENT:

IDRIR = Headers
CC = mpicc 
CFLAGS= -Wall -c
commonFlags = -Wall  #-pedantic
dependendecyFlags = -c

buildDir = Build/

allObjects = wg.o rg.o s.o p.o c.o
writeGraphObjects = SubGraph.o WriteGraph.o
readGraphObjects = SubGraph.o ReadGraph.o
serialObjects = TList.o TArray.o ListGraph.o SCCResult.o SubGraph.o Tarjan.o Serial.o
parallelObjects = TList.o TArray.o ListGraph.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o Parallel.o
compareObjects = TList.o TArray.o CompareResults.o SCCResult.o
testObjects = wg.o rg.o s.o p.o c.o

# Adding the build directory
allObjects := $(foreach object,$(allObjects),$(buildDir)$(object))
writeGraphObjects := $(foreach object,$(writeGraphObjects),$(buildDir)$(object))
readGraphObjects := $(foreach object,$(readGraphObjects),$(buildDir)$(object))
serialObjects := $(foreach object,$(serialObjects),$(buildDir)$(object))
parallelObjects := $(foreach object,$(parallelObjects),$(buildDir)$(object))
compareObjects := $(foreach object,$(compareObjects),$(buildDir)$(object))
testObjects := $(foreach object,$(testObjects),$(buildDir)$(object))

.PHONY: help test all clean cleanBin cleanObj cleanTxt optimize1 optimize2 optimize3 optimize4 optimize5 optimize6

all:  $(allObjects) 

test: $(testObjects) runTest cleanBin cleanTxt

clean: cleanObj cleanBin cleanTxt

optimize1: updateO1 all

optimize2: updateO2 all

optimize3: updateO3 all

optimize4: updateO4 all

optimize5: updateO5 all

optimize6: updateO6 all

help: 
	echo "Builds all the binaries required to test the program"
	echo "Usage:\t make all\t-the default way to execute make, builds all the required files"
	echo "\t make test\t-runs various tests of the program"
	echo "\t make clean\t-cleans the working directory, deleting object files and eventual output files"
	echo "\t make cleanObj\t-cleans the working directory, deleting the object files"
	echo "\t make cleanBin\t-cleans the working directory, deleting the binary output files"
	echo "\t make cleanTxt\t-cleans the working directory, deleting the textual output files"
	echo "\t make optimizeX\t-builds all the required files allpying the specified level of optimization X=[1-6]"
	echo "\t make help\t-prints this output"

cleanObj:
	-rm Build/*.o

cleanBin:
	-rm Data/*.bin

cleanTxt: 
	-rm Data/*.txt

runTest:
	./test.sh -t

updateO1:
	$(eval CC = $(CC) -O1)

updateO2:
	$(eval CC = $(CC) -O2)

updateO3:
	$(eval CC = $(CC) -O3)

updateO4:
	$(eval CC = $(CC) -O4)

updateO5:
	$(eval CC = $(CC) -O5)

updateO6:
	$(eval CC = $(CC) -O6)

Build/wg.o :	$(writeGraphObjects)
	$(CC) $(commonFlags) $(writeGraphObjects) -o Build/wg.o

Build/rg.o :	$(readGraphObjects)
	$(CC) $(commonFlags) $(readGraphObjects) -o Build/rg.o

Build/s.o : 	$(serialObjects)
	$(CC) $(commonFlags) $(serialObjects) -o Build/s.o 
    
Build/p.o : 	$(parallelObjects)
	$(CC) $(commonFlags) $(parallelObjects) -o Build/p.o

Build/c.o : 	$(compareObjects)
	$(CC) $(commonFlags) $(compareObjects) -o Build/c.o

Build/SubGraph.o : Source/DataStructures/SubGraph.c 
	$(CC) $(CFLAGS) Source/DataStructures/SubGraph.c -o Build/SubGraph.o

Build/TList.o : Source/DataStructures/TList.c
	$(CC) $(CFLAGS) Source/DataStructures/TList.c -o Build/TList.o

Build/TArray.o : Source/DataStructures/TArray.c
	$(CC) $(CFLAGS) Source/DataStructures/TArray.c -o Build/TArray.o

Build/SCCResult.o : Source/DataStructures/SCCResult.c 
	$(CC) $(CFLAGS) Source/DataStructures/SCCResult.c -o Build/SCCResult.o

Build/ListGraph.o : Source/DataStructures/ListGraph.c
	$(CC) $(CFLAGS) Source/DataStructures/ListGraph.c -o Build/ListGraph.o

Build/WriteGraph.o : Source/Parallel/WriteGraph.c    
	$(CC) $(CFLAGS) Source/Parallel/WriteGraph.c -o Build/WriteGraph.o

Build/ReadGraph.o : Source/Parallel/ReadGraph.c
	$(CC) $(CFLAGS) Source/Parallel/ReadGraph.c -o Build/ReadGraph.o

Build/Parallel.o : Source/Parallel/Parallel.c
	$(CC) $(CFLAGS) Source/Parallel/Parallel.c -o Build/Parallel.o

Build/Communication.o : Source/Communication/Communication.c
	$(CC) $(CFLAGS) Source/Communication/Communication.c -o Build/Communication.o

Build/Merge.o : Source/Communication/Merge.c
	$(CC) $(CFLAGS) Source/Communication/Merge.c -o Build/Merge.o 

Build/Tarjan.o : Source/Tarjan/Tarjan.c
	$(CC) $(CFLAGS) Source/Tarjan/Tarjan.c -o Build/Tarjan.o

Build/Serial.o : Source/Serial/Serial.c 
	$(CC) $(CFLAGS) Source/Serial/Serial.c  -o Build/Serial.o

Build/CompareResults.o : Source/CompareResults.c
	$(CC) $(CFLAGS) Source/CompareResults.c -o Build/CompareResults.o
