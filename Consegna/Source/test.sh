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

##gcc -c DataStructures/SubGraph.c
##gcc -c Parallel/writeGraph.c
##mpicc SubGraph.o writeGraph.o -o wg
mpirun -np $1 ./wg

if [ "$serial" -ne 0 ]; then
    # This program reads all the binary files and creates a single graph, writing it into a text file
    # so that the serial version can be used to compare the results
    ##gcc -c Parallel/readGraph.c
    ##mpicc SubGraph.o readGraph.o -o rg
    mpirun -np $1 ./rg

    # The serial program executes and writes its result on a file
<<<<<<< Updated upstream
    gcc -c DataStructures/TList.c 
    gcc -c DataStructures/TArray.c 
    gcc -c DataStructures/SCCResult.c 
    gcc -c DataStructures/ListGraph.c
    gcc -c Tarjan/Tarjan.c
    gcc -c Serial/serial.c 
    gcc TList.o TArray.o SCCResult.o ListGraph.o SubGraph.o Tarjan.o serial.o -o s && ./s
fi

# The parallel program executes and writes its result on a file
gcc -c Communication/Communication.c
gcc -c Communication/Merge.c
gcc -c Parallel/parallel.c 
mpicc TList.o TArray.o SCCResult.o SubGraph.o ListGraph.o Tarjan.o Communication.o Merge.o parallel.o -o p
=======
    ##gcc -c DataStructures/TList.c 
    ##gcc -c DataStructures/TArray.c 
    ##gcc -c DataStructures/SCCResult.c 
    ##gcc -c Tarjan/Tarjan.c
    ##gcc -c Serial/serial.c 
    ##gcc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o serial.o -o s 
    ./s
fi

# The parallel program executes and writes its result on a file
##gcc -c Communication/Communication.c
##gcc -c Communication/Merge.c
##gcc -c Parallel/parallel.c 
##mpicc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o -o p
>>>>>>> Stashed changes
mpirun -np $1 ./p

if [ "$serial" -ne 0 ]; then
    # The results of both the serial and the parallel version are compared
    ##gcc -c compareResults.c
    ##gcc TList.o TArray.o compareResults.o SCCResult.o -o c 
    ./c
fi

# rm -f *.bin *.o *.txt p s c rg wg 
rm -f *.o  p s c rg wg 

exit 0