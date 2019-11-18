#include <stdio.h>
#include <stdlib.h>
#include <float.h>

Node* readNod(char* filename, float* xMa, float* yMa)
{
    //Declarations
    Node puff;
    Node* nodes;
    int i, bSize = 100;
    int NodeCount = 0;
    float xMax = FLT_MIN, yMax = FLT_MIN;

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
        if(puff.y > yMax) yMax = puff.y;

        /*printf("\n%d\t%f\t%f\t%hu", NodeCount, puff.x, puff.y, puff.NumSegm);
        for (i=0;i<puff.NumSegm;i++)
            printf("\t%d",puff.SgmList[i]);*/

        //Checking if the allocated memory is enough, and if not, increasing the allocated space
        if(NodeCount > bSize - 1){
            bSize += 100;
            if(!realloc(nodes, bSize * sizeof(Node))){
                fprintf(stderr,"Could not allocate memory!");
                exit(2);
            }
        }

        //Copying the path from puffer to the allocated memory
        nodes[NodeCount] = puff;

        NodeCount++;
    }

    fclose(f);

    *yMa = yMax;
    *xMa = xMax;

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
            if(!realloc(segments, bSize * sizeof(Segment))){
                fprintf(stderr,"Could not allocate memory!");
                exit(2);
            }
        }

        //Copying the path from puffer to the allocated memory
        segments[SegmentCount] = puff;

        SegmentCount++;
    }

    /*printf("%d\n", PathCount);*/

    fclose(f);

    return segments;
}

void readInputs(Node** nodes, Segment** segments, int* carsInSim, char* pth, int* jmp, int* rnd, int* TMax,  float* xMa, float* yMa, float* R){
    FILE* fin;

    char nod[20], sgm[20];

    if( (fin = fopen("input.txt", "r")) == NULL)
    {
        fprintf(stderr, "No input file!");
        exit(1);
    }

    char trash[100];

    fscanf(fin,"%s\n%s\n%s\n%s%d\n%s%d\n%s%d\n%s%d\n%s%fl\n", nod, sgm, pth, trash, carsInSim, trash, jmp, trash, rnd, trash, TMax, trash, R);

    //printf("%s\n%s\n%s\n%d\n%d\n", nod, sgm, pth, *carsInSim, *jmp);

    fclose(fin);

    *nodes = readNod(nod, xMa, yMa);
    *segments = readSgm(sgm);
}
