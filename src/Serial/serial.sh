gcc -c ../DataStructures/TList.c 
gcc -c ../DataStructures/TArray.c 
gcc -c ../DataStructures/SCCResult.c 
gcc -c ../DataStructures/SubGraph.c
gcc -c ../Tarjan/Tarjan.c
gcc -c ../Parallel/utils.c
gcc -c serial.c 
gcc utils.o TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o serial.o -o serial
./serial