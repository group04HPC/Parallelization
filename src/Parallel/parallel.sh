mpicc -c ../DataStructures/SubGraph.c
mpicc -c writeGraph.c
mpicc SubGraph.o writeGraph.o -o wg
mpirun -np $1 ./wg
mpicc -c ../DataStructures/TList.c 
mpicc -c ../DataStructures/TArray.c 
mpicc -c ../DataStructures/SCCResult.c 
mpicc -c ../Tarjan/Tarjan.c
mpicc -c parallel.c 
mpicc TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o parallel.o -o parallel
mpirun -np $1 ./parallel

