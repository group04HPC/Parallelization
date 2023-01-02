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

all: $(allObjects)

test: $(testObjects)

clean:
	-rm -f *.bin *.o *.txt p s c rg wg 

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




