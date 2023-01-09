gcc -c ../../Source/DataStructures/TList.c
gcc -c ../../Source/DataStructures/TArray.c
gcc -c ../../Source/DataStructures/SubGraph.c
gcc -c ../../Source/DataStructures/ListGraph.c
gcc -c ../../Source/DataStructures/SCCResult.c
gcc TList.o TArray.o SubGraph.o ListGraph.o SCCResult.o Kosaraju.c -o Kosaraju
./Kosaraju