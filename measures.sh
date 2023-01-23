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
# This file contains the script used to make the measures
#!/bin/bash

TIME_STAMP=$(date +%s)
NMEASURES=3
MAX_PROC=4

ARRAY_RC=(400 800 1200 1600 2000 2400)

ARRAY_THS=(0 1 2 4 8)

# edges shrink factor
ARRAY_SHRINK=(1 2 4) # try with all the edges, then with the half and then with the quarter

trap "exit" INT

# Get script path
SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

for shrink in "${ARRAY_SHRINK[@]}"; do

	for size in "${ARRAY_RC[@]}"; do

		for ths in "${ARRAY_THS[@]}"; do
			ths_str=$(printf "%02d" $ths)

			OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
			OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv

			mkdir -p $(dirname $OUT_FILE) 2> /dev/null
			mkdir -p $(dirname $OUT_FILE2) 2> /dev/null

			echo $(basename $OUT_FILE)

				if [[ $ver -gt 1 && $ths -eq 0 ]]; then
					OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
					ln -srf -T $OLD_OUT_FILE $OUT_FILE
					echo Created symbolic link to $(basename $OLD_OUT_FILE)
					OLD_OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
					ln -srf -T $OLD_OUT_FILE2 $OUT_FILE2
					echo Created symbolic link to $(basename $OLD_OUT_FILE2)
					continue
				fi

			echo "size,processes,read,SCC,elapsed" >$OUT_FILE
			echo "size,processes,read,SCC,elapsed" >$OUT_FILE2

			for ((i = 0 ; i < $NMEASURES; i++)); do
				
				if [[ $ths -eq 0 ]]; then
					# serial

					./Source/updateConstants.sh $(($size/$MAX_PROC)) $(($size/$shrink)) $(($size/$shrink))
					make -B
					make cleanBin

					# write graphs
					mpirun -np $MAX_PROC ./Build/wg.o
					# read graphs and write on file th whole matrix
					mpirun -np $MAX_PROC ./Build/rg.o

					#execute serial
					echo $size,$ths,$(./Build/s.o) >> $OUT_FILE
					echo $size,$ths,$(./Build/s_k.o) >> $OUT_FILE2

					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				else
					# parallel
					new=$(($size/$ths))
					./Source/updateConstants.sh $new $(($size/$shrink)) $(($size/$shrink))
					make -B
					make cleanBin

					# write graphs
					mpirun -np $ths ./Build/wg.o
					
					#execute parallel
					echo $size,$ths,$(mpirun -np $ths ./Build/p.o) >> $OUT_FILE
					echo $size,$ths,$(mpirun -np $ths ./Build/p_k.o) >> $OUT_FILE2

					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				fi

			done

			printf "\n"
			
		done
	done
done

# we choose a size on which test the optimization levels
# TEST_SIZE=400
# ARRAY_OPT=(1 2 3)
# SHRINK=2

# for level in "${ARRAY_OPT[@]}"; do

# 	opt=$(printf "%d" $level)
# 	opt="optimize"$opt
# 	echo $opt

# 	for ths in "${ARRAY_THS[@]}"; do

# 		ths_str=$(printf "%02d" $ths)

# 		OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
# 		OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv

# 		mkdir -p $(dirname $OUT_FILE) 2> /dev/null
# 		mkdir -p $(dirname $OUT_FILE2) 2> /dev/null

# 		echo $(basename $OUT_FILE)

# 		if [[ $ver -gt 1 && $ths -eq 0 ]]; then
# 			OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
# 			ln -srf -T $OLD_OUT_FILE $OUT_FILE
# 			echo Created symbolic link to $(basename $OLD_OUT_FILE)
# 			OLD_OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
# 			ln -srf -T $OLD_OUT_FILE2 $OUT_FILE2
# 			echo Created symbolic link to $(basename $OLD_OUT_FILE2)
# 			continue
# 		fi

#         echo "size,processes,read,SCC,elapsed" >$OUT_FILE
# 		echo "size,processes,read,SCC,elapsed" >$OUT_FILE2

# 		for ((i = 0 ; i < $NMEASURES; i++)); do
			
# 			if [[ $ths -eq 0 ]]; then
# 				# serial

# 				./Source/updateConstants.sh $(($TEST_SIZE/$MAX_PROC)) $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
# 				make $opt -B
# 				make cleanBin

# 				# write graphs
# 				mpirun -np $MAX_PROC ./Build/wg.o
# 				# read graphs and write on file th whole matrix
# 				mpirun -np $MAX_PROC ./Build/rg.o

# 				#execute serial
# 				echo $TEST_SIZE,$ths,$(./Build/s.o) >> $OUT_FILE
# 				echo $TEST_SIZE,$ths,$(./Build/s_k.o) >> $OUT_FILE2

# 				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
# 			else
# 				# parallel
# 				new=$(($TEST_SIZE/$ths))
# 				./Source/updateConstants.sh $new $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
# 				make $opt -B
# 				make cleanBin

# 				# write graphs
# 				mpirun -np $ths ./Build/wg.o
				
# 				#execute parallel
# 				echo $TEST_SIZE,$ths,$(mpirun -np $ths ./Build/p.o) >> $OUT_FILE
# 				echo $TEST_SIZE,$ths,$(mpirun -np $ths ./Build/p_k.o) >> $OUT_FILE2

# 				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
# 			fi

# 		done

#         printf "\n"

# 	done
# done
