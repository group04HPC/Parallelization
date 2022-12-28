# !/bin/bash

# Check if the number of arguments is correct
if [ $# -le 0 ]; then
    echo "You have to specify the number of processes!"
    exit 1
fi

# Each process creates its own binary file with a part of the full graph
gcc -c DataStructures/SubGraph.c
gcc -c Parallel/writeGraph.c
mpicc SubGraph.o writeGraph.o -o wg
mpirun -np $1 ./wg

# This program reads all the binary files and creates a single graph, writing it into a text file
# so that the serial version can be used to compare the results
gcc -c Parallel/readGraph.c
mpicc SubGraph.o readGraph.o -o rg
mpirun -np $1 ./rg

# The serial program executes and writes its result on a file
gcc -c DataStructures/TList.c 
gcc -c DataStructures/TArray.c 
gcc -c DataStructures/SCCResult.c 
gcc -c Tarjan/Tarjan.c
gcc -c Serial/serial.c 
gcc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o serial.o -o s && ./s

# The parallel program executes and writes its result on a file
gcc -c Communication/Communication.c
gcc -c Communication/Merge.c
gcc -c Parallel/parallel.c 
mpicc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o -o p
mpirun -np $1 ./p

# The results of both the serial and the parallel version are compared
gcc -c compareResults.c
gcc TList.o TArray.o compareResults.o SCCResult.o -o c && ./c

rm -f *.bin *.o *.txt p s c rg wg 

exit 0