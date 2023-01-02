mpicc -c ../DataStructures/SubGraph.c
mpicc -c writeGraph.c
mpicc SubGraph.o writeGraph.o -o wg
mpirun -np $1 ./wg

mpicc -c readGraph.c
mpicc SubGraph.o readGraph.o -o rg
mpirun -np $1 ./rg