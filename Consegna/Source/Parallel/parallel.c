#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>
#include "../../Headers/SubGraph.h"
#include "../../Headers/SCCResult.h"
#include "../../Headers/Tarjan.h"
#include "../../Headers/Constants.h"
#include "../../Headers/Merge.h"
#include "../../Headers/Communication.h"

int nextAvailableRank(bool* values, int size, int rank);
int prevAvailableRank(bool* values, int size, int rank);
void updateAvailableRanks(bool*values, int size);
int countAvailableRanks(bool* values, int size);

int main(int argc, char* argv[]){

    /* MPI init */
    int rank, size;
    MPI_Status status;
    MPI_File fh;
    MPI_Offset offset;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    bool even = size % 2 == 0 ? true : false;
    double start, end;

    /* subgraph creation */
    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    int* edges = getEdges(sub, 0);

    /* filenames init */
    char filename[FILENAME_LENGTH], num[NUM_LENGTH];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin", EXTENSION_LENGTH);

    start = MPI_Wtime();

    /* each process reads its own subgraph from its own file */
    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD*WORK_LOAD*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    /* Tarjan algorithm */
    ListGraph* list = createListGraphFromMatrix(sub);
    // if(rank == 0){
    //     printf("List graph:\n");
    //     printListGraph(list);
    // }

    SCCResult* result = SCC(&list);
    // if(rank == 0){
    //     printf("List graph:\n");
    //     printListGraph(list);
    // }
    destroySubGraph(sub);


    if (size > 1){
        int shrink = list->nV - result->nV, recivedShrink=0;
        SubGraph *receivedGraph = NULL, *mergedGraph = NULL;
        SCCResult *receivedResult = NULL, *mergedResult = NULL, *tarjan=NULL;
        ListGraph *receivedList = NULL, *mergedList = NULL;
        
        bool values[size];
        for (int i=0; i<size; i++)
            values[i] = true;

        int next, prev, curr_av = 1, i=0;

        while(curr_av > 0){

            curr_av = countAvailableRanks(values, rank);

            if (curr_av % 2 == 0){
                // send to next
                next = nextAvailableRank(values, size, rank);
                if (next != -1){
                    sub = createMatrixGraphFromList(list);
                    // printf("Sending result:\n");
                    // SCCResultPrint(result);
                    // printf("Sending sub:\n");
                    // printSubGraph(sub);
                    send_all(sub, result, shrink, next);
                    // printf("send, i: %d,rank %d\n",i, rank);
                    break;
                }

            }else{
                // rcv from prev
                prev = prevAvailableRank(values, size, rank);

                recv_all(&receivedGraph, &receivedResult, &recivedShrink, prev);
                // printf("rcv, i: %d,rank %d\n",i, rank);
                receivedGraph->offset = receivedGraph->offset/WORK_LOAD;
                receivedResult->offset = receivedGraph->offset;
                receivedList = createListGraphFromMatrix(receivedGraph);
                if (rank == 3){
                    // printf("Received result:\n");
                    // SCCResultPrint(receivedResult);
                    // printf("Received list:\n");
                    // printListGraph(receivedList);

                    // printf("Current result:\n");
                    // SCCResultPrint(result);
                    // printf("Current list:\n");
                    // printListGraph(list);
                    
                }

                mergedResult = mergeResults(receivedResult, result);
                if (rank == 3){
                    // printf("Merged result:\n");
                    // SCCResultPrint(mergedResult);
                }
                mergedList = mergeGraphs(receivedList, list, recivedShrink, shrink, mergedResult);
                if (rank == 3){
                    // printf("Merged list:\n");
                    // printListGraph(mergedList);
                }
                
                result = SCC(&mergedList);
                if (rank == 3){
                //     printf("Tarjan result:\n");
                //     SCCResultPrint(result);
                }
                result = SCCResultCombine(result, mergedResult);
                if (rank == 3){
                    // printf("Combined result:\n");
                    // SCCResultPrint(result);
                }

                list = mergedList;
                shrink = list->nV - result->nV;
            }

            updateAvailableRanks(values, size);
            i++; 
        }
    }
    
    end = MPI_Wtime() - start;

    if (rank == size-1){
        printf("Tarjan excution time parallel: %f\n", end);

        // SCCResultPrint(result);

        FILE* f = fopen("result.txt", "a+");
        if (f == NULL){
            printf("Error opening file!\n");
            exit(1);
        }
        fprintf(f, "\n%d\n", result->nMacroNodes);
        for (int i=0; i<result->nMacroNodes; i++){
            TList list = *result->vertices[i];
            fprintf(f, "%d ", listCount(list));
            while (list != NULL){
                fprintf(f, "%d ", list->value);
                list = list->link;
            }
            fprintf(f, "\n");
        }
        fclose(f);
    }

    MPI_Finalize();
    exit(EXIT_SUCCESS);
}

int nextAvailableRank(bool* values, int size, int rank){
    for(int i=rank+1; i<size; i++){
        if (values[i]){
            return i;
        }
    }
    return -1;
}

int prevAvailableRank(bool* values, int size, int rank){
    for(int i=rank-1; i>=0; i--){
        if (values[i]){
            return i;
        }
    }
    return -1;
}

void updateAvailableRanks(bool*values, int size){
    int lastTrue=-1;

    for(int i=0;i<size;i++){
        if(values[i] && lastTrue==-1){
            lastTrue=i;
        }else if(values[i] && lastTrue!=-1){
            values[lastTrue]=false;
            lastTrue=-1;
        }
    }
}

int countAvailableRanks(bool* values, int size){
    int count = 0;
    for (int i=0; i<size; i++){
        if (values[i]){
            count++;
        }
    }
    return count;
}