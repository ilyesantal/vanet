#include <stdio.h>
#include <stdlib.h>
#include <float.h>

Path* readPth(char* filename, int* pathCount)
{
    //Declarations
    Path puff;
    Path* paths;
    int i, bSize = 100;

    FILE* f;

    //Opening the file
    if( (f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "No such file!");
        exit(1);
    }

    //Allocating memory for *MaxSize* paths
    paths = malloc(bSize*sizeof(Path));

    //Reading the lines from input file to a buffer
    while((fscanf(f, "\n%hu\n%hu", &puff.Start, &puff.NSegm)) != EOF)
    {
        //Checking if the allocated memory is enough, and if not, increasing the allocated space
        if(*pathCount > bSize - 1){
            bSize += 100;
            realloc(paths, bSize);
        }

        printf("NSegm: %d\n", puff.NSegm);
        puff.SgmList = malloc(puff.NSegm*sizeof(unsigned short));

        for (i=0; i<puff.NSegm; i++)
            fscanf(f,"\t%hu", puff.SgmList + i);

        /*printf("\n%d\t%d", puff.Start, puff.NSegm);
        for (i=0;i<puff.NSegm;i++)
            printf("\t%d",puff.SgmList[i]);*/


        //Copying the path from puffer to the allocated memory
        paths[*pathCount] = puff;

        free(puff.SgmList);

        (*pathCount)++;
    }

    fclose(f);

    return paths;
}

Node* readNod(char* filename, float* xMa, float* xMi, float* yMa, float* yMi)
{
    //Declarations
    Node puff;
    Node* nodes;
    int i, bSize = 100;
    int NodeCount = 0;
    float xMax = FLT_MIN, yMax = FLT_MIN, xMin = FLT_MAX, yMin = FLT_MAX;

    FILE* f;

    //Opening the file
    if( (f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "No such file!");
        exit(1);
    }

    //Allocating memory for *MaxSize* nodes
    nodes = malloc(bSize*sizeof(Node));

    //Reading the lines from input file to a buffer
    while((fscanf(f, "\n%f\t%f\t%hu", &puff.x, &puff.y, &puff.NumSegm)) != EOF)
    {
        puff.SgmList = malloc(puff.NumSegm*sizeof(unsigned short));
        for (i=0; i<puff.NumSegm; i++)
            fscanf(f,"\t%hu", puff.SgmList + i);

        if(puff.x > xMax) xMax = puff.x;
        if(puff.x < xMin) xMin = puff.x;
        if(puff.y > yMax) yMax = puff.y;
        if(puff.y < yMin) yMin = puff.y;

        /*printf("\n%d\t%f\t%f\t%hu", NodeCount, puff.x, puff.y, puff.NumSegm);
        for (i=0;i<puff.NumSegm;i++)
            printf("\t%d",puff.SgmList[i]);*/

        //Checking if the allocated memory is enough, and if not, increasing the allocated space
        if(NodeCount > bSize - 1){
            bSize += 100;
            realloc(nodes, bSize);
        }

        //Copying the path from puffer to the allocated memory
        nodes[NodeCount] = puff;

        NodeCount++;
    }

    fclose(f);

    *yMa = yMax;
    *yMi = yMin;
    *xMa = xMax;
    *xMi = xMin;

    return nodes;
}

Segment* readSgm(char* filename)
{
    //Declarations
    Segment puff;
    Segment* segments;
    int SegmentCount = 0, bSize = 100;

    FILE* f;

    //Opening the file
    if( (f = fopen(filename, "r")) == NULL)
    {
        fprintf(stderr, "No such file!");
        exit(1);
    }

    //Allocating memory for *MaxSize* segments
    segments = malloc(bSize*sizeof(Segment));

    //Reading the lines from input file to a buffer
    while((fscanf(f, "%hu\t%hu\t%hu\n", &puff.N1, &puff.N2, &puff.type)) != EOF)
    {

        /*printf("%d\t%d\t%d\t%d\n", SegmentCount, puff.N1, puff.N2, puff.type);*/

        //Checking if the allocated memory is enough, and if not, increasing the allocated space
        if(SegmentCount > bSize - 1){
            bSize += 100;
            realloc(segments, bSize);
        }

        //Copying the path from puffer to the allocated memory
        segments[SegmentCount] = puff;

        SegmentCount++;
    }

    /*printf("%d\n", PathCount);*/

    fclose(f);

    return segments;
}

void readInputs(Node** nodes, Segment** segments, int* carsInSim, char* pth, int* jmp, int* rnd, int* TMax,  float* xMa, float* xMi, float* yMa, float* yMi){
    FILE* fin;

    char nod[20], sgm[20];

    if( (fin = fopen("input.txt", "r")) == NULL)
    {
        fprintf(stderr, "No input file!");
        exit(1);
    }

    char trash[100];

    fscanf(fin,"%s\n%s\n%s\n%s%d\n%s%d\n%s%d\n%s%d\n", nod, sgm, pth, trash, carsInSim, trash, jmp, trash, rnd, trash, TMax);

    //printf("%s\n%s\n%s\n%d\n%d\n", nod, sgm, pth, *carsInSim, *jmp);

    fclose(fin);

    *nodes = readNod(nod, xMa, xMi, yMa, yMi);
    *segments = readSgm(sgm);
}
