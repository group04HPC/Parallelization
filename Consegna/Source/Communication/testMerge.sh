gcc -c ../DataStructures/TList.c 
gcc -c ../DataStructures/TArray.c 
gcc -c ../DataStructures/SCCResult.c 
gcc -c ../DataStructures/SubGraph.c
gcc -c ../DataStructures/ListGraph.c
gcc -c ../Tarjan/Tarjan.c
gcc -c Merge.c
gcc -c testMerge.c 
gcc TList.o TArray.o SCCResult.o SubGraph.o Merge.o Tarjan.o ListGraph.o testMerge.o -o testMerge
./testMerge