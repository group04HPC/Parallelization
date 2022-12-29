#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mpi.h>
#include "../DataStructures/SubGraph.h"
#include "../DataStructures/SCCResult.h"
#include "../Tarjan/Tarjan.h"
#include "../Constants.h"
#include "../Communication/Merge.h"
#include "../Communication/Communication.h"

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

    /* subgraph creation */
    SubGraph* sub = createSubGraph(WORK_LOAD, WORK_LOAD*size, rank);
    int* edges = getEdges(sub, 0);

    /* filenames init */
    char filename[FILENAME_LENGTH], num[NUM_LENGTH];
    strcpy(filename, "file");
    sprintf(num, "%d", rank);
    strncat(filename, num, MEDIUM_FILENAME_LENGTH);
    strncat(filename, ".bin", EXTENSION_LENGTH);

    /* each process reads its own subgraph from its own file */
    MPI_Comm file_comm;
    MPI_Comm_split(MPI_COMM_WORLD, rank, rank, &file_comm);
    MPI_File_open(file_comm, filename, MPI_MODE_CREATE | MPI_MODE_RDWR, MPI_INFO_NULL, &fh);
    MPI_File_seek(fh, WORK_LOAD*WORK_LOAD*size, MPI_SEEK_SET);
    MPI_File_read(fh, edges, WORK_LOAD*WORK_LOAD*size, MPI_INT, &status);
    MPI_File_close(&fh);
    MPI_Comm_free(&file_comm);

    /* Tarjan algorithm */
    SCCResult* result = SCCResultRescale(SCC(sub));
    int shrink = 0;
    //printf("%d nV, %d nMacroNodes, %d rank\n", sub->nV, result->nV, rank);
    shrink = sub->nV - result->nV;
    if (sub->nV != result->nV)
        sub = rescaleGraph(sub, result, result, 0);
    //printf("%d shrink, %d rank\n", shrink, rank);

    int start = 0, stop = even ? size/2 : (size-1)/2, recivedShrink=0;
    SubGraph *receivedGraph = NULL, *mergedGraph = NULL;
    SCCResult *receivedResult = NULL, *mergedResult = NULL, *tarjan=NULL;
    
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
                // if (rank == 1 && i==1){
                //     printf("il mio sotto grafo e':\n");
                //     printSubGraph(sub);
                // }
                send_all(sub, result, shrink, next);
                break;
            }

        }else{
            // rcv from prev
            prev = prevAvailableRank(values, size, rank);
            // printf("-------rank %d trying ro rcv from %d-------\n", rank, prev);
            recv_all(&receivedGraph, &receivedResult, &recivedShrink, prev);
            // printf("-------rank %d receiving from %d-------\n", rank, prev);
            receivedResult->offset = receivedGraph->offset;
            if (rank == 3 && i==1){
                printf("Received graph:\n");
                printSubGraph(receivedGraph);
                printf("Received result:\n");
                SCCResultPrint(receivedResult);
                printf("My graph:\n");
                printSubGraph(sub);
                printf("My result:\n");
                SCCResultPrint(result);
            }

            mergedResult = mergeResults(receivedResult, result);
            if (rank == 3 && i==1){
                printf("Merged result:\n");
                SCCResultPrint(mergedResult);
            }
            
            mergedGraph = mergeGraphs(receivedGraph, sub, recivedShrink, shrink, mergedResult);
            if (rank == 3 && i==1){
                printf("Merged graph:\n");
                printSubGraph(mergedGraph);
            }
            //mergedGraph = receivedGraph;
            sub = mergedGraph;
            result = SCCResultRescale(SCC(mergedGraph));
            tarjan = result;
            if (rank == 3 && i==1){
                printf("Tarjan result:\n");
                SCCResultPrint(tarjan);
            }

            shrink = sub->nV-result->nV;
            result = SCCResultCombine(result, mergedResult);
            if (rank == 3 && i==1){
                printf("Combined result:\n");
                SCCResultPrint(result);
            }
            if (sub->nV != result->nV){
                if (rank == 3 && i==1){
                    // printf("=============\n");
                    printf("Merged graph:\n");
                    printSubGraph(mergedGraph);
                    // printf("Tarjan result:\n");
                    // SCCResultPrint(tarjan);
                    // printf("Merged result:\n");
                    // SCCResultPrint(mergedResult);
                }

                sub = rescaleGraph(mergedGraph, tarjan, mergedResult, 1);

                if (rank == 3 && i==1){
                    printf("--3-- Rescaled graph:\n");
                    printSubGraph(sub);
                }
            }
        }

        updateAvailableRanks(values, size);
        i++; 
    }

    // printf("rank %d finished\n", rank);

    if (rank == size-1){
        printf("Final graph:\n");
        printSubGraph(sub);
        printf("\nFinal result:\n");
        int max = SCCResultGetLastElement(result);
        printf("Max: %d\n", max);
        SCCResultPrint(result);

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