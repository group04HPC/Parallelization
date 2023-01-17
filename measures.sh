#!/bin/bash

TIME_STAMP=$(date +%s)
NMEASURES=3
MAX_PROC=16

ARRAY_RC=(2800 3200 3600 4000 4400)
ARRAY_THS=(0 1 2 4 8 16)

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

			mkdir -p $(dirname $OUT_FILE) 2> /dev/null

			echo $(basename $OUT_FILE)

				if [[ $ver -gt 1 && $ths -eq 0 ]]; then
					OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
					ln -srf -T $OLD_OUT_FILE $OUT_FILE
					echo Created symbolic link to $(basename $OLD_OUT_FILE)
					continue
				fi

			echo "size,processes,read,SCC,elapsed" >$OUT_FILE

			for ((i = 0 ; i < $NMEASURES; i++)); do
				
				if [[ $ths -eq 0 ]]; then
					# serial

					./mpidir/Source/updateConstants.sh $(($size/$MAX_PROC)) $(($size/$shrink)) $(($size/$shrink))
					cd mpidir
					make -B
					make cleanBin
					cd ..

					# write graphs
					mpirun -n $MAX_PROC --hostfile mpi_host_file ./mpidir/Build/wg.o
					# read graphs and write on file th whole matrix
					mpirun -n $MAX_PROC --hostfile mpi_host_file ./mpidir/Build/rg.o

					#execute serial
					echo $size,$ths,$(./mpidir/Build/s.o) >> $OUT_FILE
					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				else
					# parallel
					new=$(($size/$ths))
					./mpidir/Source/updateConstants.sh $new $(($size/$shrink)) $(($size/$shrink))
					cd mpidir
					make -B
					make cleanBin
					cd ..

					# write graphs
					mpirun -n $ths --hostfile mpi_host_file ./mpidir/Build/wg.o
					
					#execute parallel
					echo $size,$ths,$(mpirun -n $ths --hostfile mpi_host_file ./mpidir/Build/p.o) >> $OUT_FILE
					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				fi

			done

			printf "\n"
			
		done
	done
done

# we choose a size on which test the optimization levels
TEST_SIZE=400
ARRAY_OPT=(1 2 3)
SHRINK=2

for level in "${ARRAY_OPT[@]}"; do

	opt=$(printf "%d" $level)
	opt="optimize"$opt
	echo $opt

	for ths in "${ARRAY_THS[@]}"; do

		ths_str=$(printf "%02d" $ths)

		OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv

		mkdir -p $(dirname $OUT_FILE) 2> /dev/null
		# mkdir -p $(dirname $OUT_FILE2) 2> /dev/null
		echo $(basename $OUT_FILE)

		if [[ $ver -gt 1 && $ths -eq 0 ]]; then
			OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
			ln -srf -T $OLD_OUT_FILE $OUT_FILE
			echo Created symbolic link to $(basename $OLD_OUT_FILE)
			continue
		fi

        echo "size,processes,read,SCC,elapsed" >$OUT_FILE

		for ((i = 0 ; i < $NMEASURES; i++)); do
			
			if [[ $ths -eq 0 ]]; then
				# serial

				./mpidir/Source/updateConstants.sh $(($TEST_SIZE/$MAX_PROC)) $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
				cd mpidir
				make $opt -B
				make cleanBin
				cd ..

				# write graphs
				mpirun -n $MAX_PROC --hostfile mpi_host_file ./mpidir/Build/wg.o
				# read graphs and write on file th whole matrix
				mpirun -n $MAX_PROC --hostfile mpi_host_file ./mpidir/Build/rg.o

				#execute serial
				echo $TEST_SIZE,$ths,$(./mpidir/Build/s.o) >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			else
				# parallel
				new=$(($TEST_SIZE/$ths))
				./mpidir/Source/updateConstants.sh $new $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
				cd mpidir
				make $opt -B
				make cleanBin
				cd ..

				# write graphs
				mpirun -n $ths --hostfile mpi_host_file ./mpidir/Build/wg.o
				
				#execute parallel
				echo $TEST_SIZE,$ths,$(mpirun -n $ths --hostfile ./mpi_host_file mpidir/Build/p.o) >> $OUT_FILE
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			fi

		done

        printf "\n"

	done
done
