gcc -c ../DataStructures/TList.c 
gcc -c ../DataStructures/TArray.c 
gcc -c ../DataStructures/SCCResult.c 
gcc -c ../DataStructures/SubGraph.c
gcc -c Tarjan.c
gcc -c testTarjan.c 
gcc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o testTarjan.o -o testTarjan
./testTarjan