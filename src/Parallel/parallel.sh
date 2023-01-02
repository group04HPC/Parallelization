mpicc -c ../DataStructures/TList.c 
mpicc -c ../DataStructures/TArray.c 
mpicc -c ../DataStructures/SubGraph.c
mpicc -c ../DataStructures/SCCResult.c 
mpicc -c ../DataStructures/ListGraph.c
mpicc -c ../Tarjan/Tarjan.c
mpicc -c ../Communication/Communication.c
mpicc -c ../Communication/Merge.c
mpicc -c parallel.c 
mpicc TList.o TArray.o SCCResult.o ListGraph.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o -o parallel
mpirun -np $1 ./parallel

# Initialize variables for the loop indices
# i=0
# j=0
# iterations=10

# # Loop until the end of the first array is reached
# while [ $i -lt $iterations ]
# do
#   # Loop until the end of the second array is reached
    

#   # Reset the second loop index and increment the first loop index
#   j=0
#   i=$((i+1))
# done