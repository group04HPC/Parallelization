CC = mpicc #gcc
CFLAGS = -Wall -c #-pedantic
objects = f.o r.o

writeGraphObjects = SubGraph.o writeGraph.o
readGraphObjects = SubGraph.o readGraph.o
serialObjects = TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o Serial.o
parallelObjects = TList.o TArray.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o
compareObjects = TList.o TArray.o compareResults.o SCCResult.o

.PHONY: all
all: 

	Build/SubGraph.o: Source/DataStructures/SubGraph.c

	writeGraph.o: Source/Parallel/writeGraph.c    

	readGraph.o: Source/Parallel/readGraph.c

	TList.o: Source/DataStructures/TList.c

	TArray.o: Source/DataStructures/TArray.c

	SCCResult.o: Source/DataStructures/SCCResult.c 

	Tarjan.o: Source/Tarjan/Tarjan.c

	Serial.o: Source/Serial/serial.c 

	Communication.o: Source/Communication/Communication.c

	Merge.o: Source/Communication/Merge.c

	parallel.o: Source/Parallel/parallel.c

	compareResults.o: Source/compareResults.c

	wg:	$(writeGraphObjects)
		$(CC) -Wall $(writeGraphObjects) -o Build/wg

	rg:	$(readGraphObjects)
		$(CC) -Wall $(readGraphObjects) -o Build/rg

	s: 	$(serialObjects)
		$(CC) -Wall $(serialObjects) -o Build/s 
    
	p: 	$(parallelObjects)
		$(CC) -Wall $(parallelObjects) -o Build/p

	c: 	$(compareObjects)
		$(CC) -Wall $(compareObjects) -o Build/c
		
.PHONY: test
test:


.PHONY: clean
clean:
	-rm -f *.bin *.o *.txt p s c rg wg 

