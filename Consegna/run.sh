# !/bin/bash

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
	echo "Running a test with a 1000x1000 matrix on 4 processes"
	./test.sh 4

	echo "Running first test with a 1000x1000 matrix on 4 processes"
	./test.sh 4

	echo "Running first test with a 1000x1000 matrix on 4 processes"
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
