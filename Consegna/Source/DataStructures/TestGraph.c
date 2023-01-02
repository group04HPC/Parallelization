#include "SubGraph.c"
#include "SCCResult.c"
#include "TList.c"
#include "TArray.c"
#include "../Tarjan/Tarjan.c"
#include "../Communication/Merge.c"

/*#include "../Communication/Merge.c"

#define VERTEX_PER_GRAPH 20

int main(int argc, char**argv){
    int v=2,k=6;
    SubGraph *g1=createSubGraph(v,k-2,0);
    generateRandomSubGraph(g1,2,3);
    printf("Graph 1: %d\n",g1->offset);
    for(int i=0;i<g1->nV;i++)
        removeEdge(g1,i,g1->nE-1);
    printSubGraph(g1);


    SubGraph *g2 = createSubGraph(1, k, 2);
    generateRandomSubGraph(g2,6,7);
    printf("Graph 2: %d\n", g2->offset);
    printSubGraph(g2);

    printf("Merged\n");
    SCCResult *res=SCCResultCreate(7);
    printSubGraph(mergeGraphs(g1,g2,res,res));
}*/

int main(int argnum, char **argv)
{
    printf("A\n");
    SubGraph *a=createSubGraph(5,10,0);
        int tempa[50] = {0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
                         0, 1, 0, 1, 0, 1, 1, 1, 1, 0,
                         0, 1, 0, 1, 0, 1, 1, 1, 1, 0,
                         0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
                         0, 1, 0, 1, 1, 1, 1, 1, 1, 0};
    a->adj = tempa;
    SCCResult *ra=SCCResultRescale(SCC(a));
    SCCResult *k=SCCResultCreate(5);
    SCCResultInsert(k, 1, 1);
    SCCResultInsert(k, 0, 0);
    SCCResultInsert(k, 2, 2);
    SCCResultInsert(k, 3, 3);
    SCCResultInsert(k, 4, 4);

    SubGraph *b = createSubGraph(5, 10, 1);
    int temp[50] = {0, 1, 0, 1, 1, 1, 1, 1, 1, 1,
                    0, 1, 0, 1, 0, 1, 1, 1, 1, 0,
                    0, 0, 0, 1, 0, 1, 1, 1, 1, 0,
                    0, 1, 0, 1, 1, 1, 1, 1, 1, 0,
                    0, 1, 0, 1, 1, 1, 1, 1, 1, 1};
    b->adj =temp;
    SCCResult *rb = SCCResultRescale(SCC(b));
    
    printf("A rescalato:\n");
    a = rescaleGraph(a, ra);
    printSubGraph(a);
    SCCResultPrint(ra);

    printf("B rescalato:\n");
    b = rescaleGraph(b, rb);
    printSubGraph(b);
    SCCResultPrint(rb);
    printf("Finale\n");
    printf("%d %d\n",10-a->nE,10-b->nE);
    SubGraph *tempo = mergeGraphs(a, b, 10 - a->nE, 10 - b->nE, mergeResults(ra, rb));
    SCCResult *res = SCC(tempo);
    SubGraph *final = rescaleGraph(tempo, SCCResultRescale(res));
    printSubGraph(final);
    SCCResultPrint(SCCResultRescale(res));
    
    /*SubGraph *a = createSubGraph(10, 10, 0);
    int temp[] = {0,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1,
                  0,
                  1,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1,
                  0,
                  0,
                  1,
                  0,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1,
                  1};
    a->adj = temp;
    SCCResult *rb = SCCResultRescale(SCC(a));
    a = rescaleGraph(a, rb, 0);

    printSubGraph(a);
    SCCResultPrint(rb);
    */
}