mpicc -c ../DataStructures/TList.c 
mpicc -c ../DataStructures/TArray.c 
mpicc -c ../DataStructures/SCCResult.c 
mpicc -c ../Tarjan/Tarjan.c
mpicc -c ../Communication/Communication.c
mpicc -c ../Communication/Merge.c
mpicc -c parallel.c 
mpicc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o -o parallel
mpirun -np $1 ./parallel

