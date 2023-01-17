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
			# OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv

			mkdir -p $(dirname $OUT_FILE) 2> /dev/null
			# mkdir -p $(dirname $OUT_FILE2) 2> /dev/null

			echo $(basename $OUT_FILE)

				if [[ $ver -gt 1 && $ths -eq 0 ]]; then
					OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
					ln -srf -T $OLD_OUT_FILE $OUT_FILE
					echo Created symbolic link to $(basename $OLD_OUT_FILE)
					# OLD_OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size-E-$shrink/SIZE-$size-NP-$ths_str-E-$shrink.csv
					# ln -srf -T $OLD_OUT_FILE2 $OUT_FILE2
					# echo Created symbolic link to $(basename $OLD_OUT_FILE2)
					continue
				fi

			echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE
			# echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE2

			for ((i = 0 ; i < $NMEASURES; i++)); do
				
				if [[ $ths -eq 0 ]]; then
					# serial

					./Source/updateConstants.sh $(($size/$MAX_PROC)) $(($size/$shrink)) $(($size/$shrink))
					make -B

					# write graphs
					mpirun -n $MAX_PROC --hostfile ../mpi_host_file .././mpidir/wg.o
					# read graphs and write on file th whole matrix
					mpirun -n $MAX_PROC --hostfile ../mpi_host_file .././mpidir/rg.o

					./Source/updateConstants.sh $size $(($size/$shrink)) $(($size/$shrink))
					make -B

					#execute serial
					echo $size,$ths,$(./mpidir/s.o) >> $OUT_FILE
					# echo $size,$ths,$(./mpidir/s_k.o) >> $OUT_FILE2
					printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
				else
					# parallel
					new=$(($size/$ths))
					./Source/updateConstants.sh $new $(($size/$shrink)) $(($size/$shrink))
					make -B

					# write graphs
					mpirun -n $ths --hostfile ../mpi_host_file .././mpidir/wg.o
					
					#execute parallel
					echo $size,$ths,$(mpirun -n $ths --hostfile ../mpi_host_file .././mpidir/p.o) >> $OUT_FILE
					# echo $size,$ths,$(mpirun -n $ths ./mpidir/p_k.o) >> $OUT_FILE2
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
		# OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv

		mkdir -p $(dirname $OUT_FILE) 2> /dev/null
		# mkdir -p $(dirname $OUT_FILE2) 2> /dev/null
		echo $(basename $OUT_FILE)

		if [[ $ver -gt 1 && $ths -eq 0 ]]; then
			OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
			ln -srf -T $OLD_OUT_FILE $OUT_FILE
			echo Created symbolic link to $(basename $OLD_OUT_FILE)
			# OLD_OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$TEST_SIZE-O-$level/SIZE-$TEST_SIZE-NP-$ths_str-O-$level.csv
			# ln -srf -T $OLD_OUT_FILE2 $OUT_FILE2
			# echo Created symbolic link to $(basename $OLD_OUT_FILE2)
			continue
		fi

        echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE
		# echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE2

		for ((i = 0 ; i < $NMEASURES; i++)); do
			
			if [[ $ths -eq 0 ]]; then
				# serial

				./Source/updateConstants.sh $(($TEST_SIZE/$MAX_PROC)) $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
				make $opt -B

				# write graphs
				mpirun -n $MAX_PROC --hostfile ../mpi_host_file .././mpidir/wg.o
				# read graphs and write on file th whole matrix
				mpirun -n $MAX_PROC --hostfile ../mpi_host_file .././mpidir/rg.o

				./Source/updateConstants.sh $TEST_SIZE $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
				make $opt -B

				#execute serial
				echo $TEST_SIZE,$ths,$(./mpidir/s.o) >> $OUT_FILE
				# echo $TEST_SIZE,$ths,$(./mpidir/s_k.o) >> $OUT_FILE2
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			else
				# parallel
				new=$(($TEST_SIZE/$ths))
				./Source/updateConstants.sh $new $(($TEST_SIZE/$SHRINK)) $(($TEST_SIZE/$SHRINK))
				make $opt -B

				# write graphs
				mpirun -n $ths --hostfile ../mpi_host_file .././mpidir/wg.o
				
				#execute parallel
				echo $TEST_SIZE,$ths,$(mpirun -n $ths --hostfile ../mpi_host_file ./mpidir/p.o) >> $OUT_FILE
				# echo $TEST_SIZE,$ths,$(mpirun -n $ths ./mpidir/p_k.o) >> $OUT_FILE2
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			fi

		done

        printf "\n"

	done
done
