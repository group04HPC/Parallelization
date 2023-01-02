CC = mpicc #gcc
CFLAGS = -Wall -c #-pedantic

buildDir = Build/

allObjects = wg rg s p c
writeGraphObjects = SubGraph.o writeGraph.o
readGraphObjects = SubGraph.o readGraph.o
serialObjects = TList.o TArray.o ListGraph.o SCCResult.o SubGraph.o Tarjan.o Serial.o
parallelObjects = TList.o TArray.o ListGraph.o SCCResult.o SubGraph.o Tarjan.o Communication.o Merge.o parallel.o
compareObjects = TList.o TArray.o compareResults.o SCCResult.o

# Adding the build directory
allObjects := $(foreach object,$(allObjects),$(buildDir)$(object))
writeGraphObjects := $(foreach object,$(writeGraphObjects),$(buildDir)$(object))
readGraphObjects := $(foreach object,$(readGraphObjects),$(buildDir)$(object))
serialObjects := $(foreach object,$(serialObjects),$(buildDir)$(object))
parallelObjects := $(foreach object,$(parallelObjects),$(buildDir)$(object))
compareObjects := $(foreach object,$(compareObjects),$(buildDir)$(object))

.PHONY: clean test all

all:  $(allObjects) #$(parallelObjects) $(serialObjects) $(allObjects)

test: $(testObjects)

clean:
	-rm Build/*

Build/wg :	$(writeGraphObjects)
	$(CC) -Wall $(writeGraphObjects) -o Build/wg

Build/rg :	$(readGraphObjects)
	$(CC) -Wall $(readGraphObjects) -o Build/rg

Build/s : 	$(serialObjects)
	$(CC) -Wall $(serialObjects) -o Build/s 
    
Build/p : 	$(parallelObjects)
	$(CC) -Wall $(parallelObjects) -o Build/p

Build/c : 	$(compareObjects)
	$(CC) -Wall $(compareObjects) -o Build/c

Build/SubGraph.o : Source/DataStructures/SubGraph.c 
	$(CC) $(CFLAGS) Source/DataStructures/SubGraph.c -o Build/SubGraph.o

Build/TList.o : Source/DataStructures/TList.c
	$(CC) $(CFLAGS) Source/DataStructures/TList.c -o Build/TList.o

Build/TArray.o : Source/DataStructures/TArray.c
	$(CC) $(CFLAGS) Source/DataStructures/TArray.c -o Build/TArray.o

Build/SCCResult.o : Source/DataStructures/SCCResult.c 
	$(CC) $(CFLAGS) Source/DataStructures/SCCResult.c -o Build/SCCResult.o

Build/ListGraph.o : Source/DataStructures/ListGraph.c
	$(CC) $(CFLAGS) Source/DataStructures/ListGraph.c -o Build/ListGraph.o

Build/writeGraph.o : Source/Parallel/writeGraph.c    
	$(CC) $(CFLAGS) Source/Parallel/writeGraph.c -o Build/writeGraph.o

Build/readGraph.o : Source/Parallel/readGraph.c
	$(CC) $(CFLAGS) Source/Parallel/readGraph.c -o Build/readGraph.o

Build/parallel.o : Source/Parallel/parallel.c
	$(CC) $(CFLAGS) Source/Parallel/parallel.c -o Build/parallel.o

Build/Communication.o : Source/Communication/Communication.c
	$(CC) $(CFLAGS) Source/Communication/Communication.c -o Build/Communication.o

Build/Merge.o : Source/Communication/Merge.c
	$(CC) $(CFLAGS) Source/Communication/Merge.c -o Build/Merge.o 

Build/Tarjan.o : Source/Tarjan/Tarjan.c
	$(CC) $(CFLAGS) Source/Tarjan/Tarjan.c -o Build/Tarjan.o

Build/Serial.o : Source/Serial/serial.c 
	$(CC) $(CFLAGS) Source/Serial/serial.c  -o Build/Serial.o

Build/compareResults.o : Source/compareResults.c
	$(CC) $(CFLAGS) Source/compareResults.c -o Build/compareResults.o


