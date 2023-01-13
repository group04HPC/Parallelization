#!/bin/bash

TIME_STAMP=$(date +%s)
NMEASURES=1

ARRAY_RC=(400 800 1200 1600 2000 2400)
ARRAY_THS=(0 1 2 4 8)

trap "exit" INT

# Get script path
SCRIPTPATH=$( cd -- "$(dirname "$0")" >/dev/null 2>&1 ; pwd -P )

for size in "${ARRAY_RC[@]}"; do

	read=$(($size/4))

	./Source/updateConstants.sh $read 0 $size
	make -B

	# write graphs
	mpirun -np 4 ./Build/wg.o
	# read graphs and write on file th whole matrix
	mpirun -np 4 ./Build/rg.o

	# update constants
	./Source/updateConstants.sh $size 0 $size
	make -B

	for ths in "${ARRAY_THS[@]}"; do
        ths_str=$(printf "%02d" $ths)

        OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size/SIZE-$size-NP-$ths_str.csv
		OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size/SIZE-$size-NP-$ths_str.csv

        mkdir -p $(dirname $OUT_FILE) 2> /dev/null
		mkdir -p $(dirname $OUT_FILE2) 2> /dev/null

        echo $(basename $OUT_FILE)

			if [[ $ver -gt 1 && $ths -eq 0 ]]; then
				OLD_OUT_FILE=$SCRIPTPATH/Measures/Tarjan/SIZE-$size-V1/SIZE-$size-NP-$ths_str.csv
				ln -srf -T $OLD_OUT_FILE $OUT_FILE
				echo Created symbolic link to $(basename $OLD_OUT_FILE)
				OLD_OUT_FILE2=$SCRIPTPATH/Measures/Kosaraju/SIZE-$size-V1/SIZE-$size-NP-$ths_str.csv
				ln -srf -T $OLD_OUT_FILE2 $OUT_FILE2
				echo Created symbolic link to $(basename $OLD_OUT_FILE2)
				continue
			fi

        echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE
		echo "size,processes,read,SCC,write,elapsed" >$OUT_FILE2

        for ((i = 0 ; i < $NMEASURES; i++)); do
			
			if [[ $ths -eq 0 ]]; then
				# serial
				echo $size,$ths,$(./Build/s.o) >> $OUT_FILE
				echo $size,$ths,$(./Build/s_k.o) >> $OUT_FILE2
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			else
				# parallel
				new=$(($size/$ths))
				./Source/updateConstants.sh $new 0 $size
				make -B
				# write graphs
				mpirun -np $ths ./Build/wg.o
				echo $size,$ths,$(mpirun -np $ths ./Build/p.o) >> $OUT_FILE
				echo $size,$ths,$(mpirun -np $ths ./Build/p_k.o) >> $OUT_FILE2
				printf "\r> %d/%d %3.1d%% " $(expr $i + 1) $NMEASURES $(expr \( \( $i + 1 \) \* 100 \) / $NMEASURES)
			fi

		done

        printf "\n"
        
    done
done