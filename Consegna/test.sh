## Tutti i ## dovrebbero essere gestiti dal makefile

# !/bin/bash

# Check if the user asked for help
if [ "$1" = "" ]; then
    echo "Executes the program on a given number of processes. If the [run serial] param is not filled it will execute it as default\n\n"
            "Usage:\t$0 [num of processes] \n"
            "\t$0 [num of processes] [run serial (0/1)]"
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

mpirun -np $1 Build/./wg

if [ "$serial" -ne 0 ]; then
    # This program reads all the binary files and creates a single graph, writing it into a text file
    # so that the serial version can be used to compare the results
    mpirun -np $1 Build/./rg

    # The serial program executes and writes its result on a file
    Build/./s
fi

# The parallel program executes and writes its result on a file
mpirun -np $1 Build/./p

if [ "$serial" -ne 0 ]; then
    # The results of both the serial and the parallel version are compared
    ##gcc -c compareResults.c
    Build/./c
fi

# rm -f *.bin *.o *.txt p s c rg wg 
# rm -f *.o  p s c rg wg 

exit 0