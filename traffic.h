#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <sys/types.h>
#include "structs.h"


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

float* createPoints(Node *nodes, Segment segment, int *prevend, int *addedPoints, float *remainingFromPrevious, float *segmentLength, float *timeRequired){
    //Declarations
    int i, points;
    float *segmentXY = NULL;
    float x, y, ex, ey, startx, starty, time, length;
    float speed[7];

    //Conversion to vector
    if(segment.N1 == *prevend) {
        x = nodes[segment.N2].x - nodes[segment.N1].x;
        y = nodes[segment.N2].y - nodes[segment.N1].y;
    }
    else {
        x = nodes[segment.N1].x - nodes[segment.N2].x;
        y = nodes[segment.N1].y - nodes[segment.N2].y;
    }

    //Calculating the length of the segment
    length = *segmentLength = sqrt(pow(x , 2) + pow(y, 2));

    //Determining speed, converting km/h to m/s
    speed[0] = 100.0*5.0/18.0;
    speed[1] = 30.0*5.0/18.0;
    speed[2] = 28.0*5.0/18.0;
    speed[3] = 26.0*5.0/18.0;
    speed[4] = 24.0*5.0/18.0;
    speed[5] = 22.0*5.0/18.0;
    speed[6] = 18.0*5.0/18.0;

    int t = segment.type;

    //How many points to add
    points = *addedPoints = (int) floor(length/speed[t]);   //Also the time (in s) required for the segment
    time = *timeRequired = length/speed[t];

    //Creating unit vector
    ex = x/time;
    ey = y/time;

    //Allocating memory for the array to return
    segmentXY = malloc(points * sizeof(float) * 2);
    //Saving the start point of the segment
    if(segment.N1 == *prevend)
    {
        startx = nodes[segment.N1].x;
        starty = nodes[segment.N1].y;
        *prevend = segment.N2;
    }
    else{
        startx = nodes[segment.N2].x;
        starty = nodes[segment.N2].y;
        *prevend = segment.N1;
    }

    //Adding the remaining "seconds" from previous segment
    startx -= *remainingFromPrevious*ex;
    starty -= *remainingFromPrevious*ey;

    int currentDist = 0;

    //Adding one point in the distance of a number of unit vectors
    for(i = 0; i < (points * 2); i ++)
    {
        if(i%2 == 0)
            segmentXY[i] = startx + currentDist*ex;
        else
            segmentXY[i] = starty + currentDist*ey;
        currentDist++;
    }

    //printf("\t\tx: %f\n\t\ty: %f\n", x, y); //Print the vector
    //printf("\t\tex: %f\n\t\tey: %f\n", ex, ey); //Print the unit vector

    //Print the coordinates of the points in the segment
    /*for(i = 0; i < ((*addedPoints) * 2); i += 2)
    {
        printf("\t\t\tx: %f, y: %f\n", segmentXY[i], segmentXY[i + 1]);
    }*/

    *remainingFromPrevious = time - points;

    return segmentXY;
}

float* convertPath(Path* path, Node* nodes, Segment* segments){
    //Declarations
    int addedPoints, i;
    float segmentLength, timeRequired;
    float remainingFromPrevious = 0;
    float *segmentPoints;
    int prevend = path->Start;

    for(i = 0; i < path->NSegm; i++)     //Going through the segments in the path
    {
        //Calling the function to convert the segments to points
        segmentPoints = createPoints(nodes, segments[path->SgmList[i]], &prevend, &addedPoints, &remainingFromPrevious, &segmentLength, &timeRequired);

        path->Duration += timeRequired;
        path->Length += segmentLength;
    }

    return segmentPoints;
}

Car getNextCar(int carsInSim, FILE** paths, Node* nodes, Segment* segments, int n){
    static int carId = 0;
    int i;
    Path path;
    Car car;

    char * line = NULL;
    size_t len = 0;

    int m = rand() % n + 1;


    for(i = 0; i < m; i++){
        if(!getline(&line, &len, *paths)){
            fseek(*paths, 0, SEEK_SET);
            i--;
        }
    }

    fscanf(*paths, "\n%hu\n%hu", &path.Start, &path.NSegm);
    //printf("%hu, %hu\n", path.Start, path.NSegm);
    path.SgmList = malloc(path.NSegm*sizeof(unsigned short));
    for (i=0; i<path.NSegm; i++){
        fscanf(*paths,"\t%hu", &path.SgmList[i]);
    }
    //printf("\n");

    car.PathXY = convertPath(&path, nodes, segments);
    car.Pos = 0;
    car.NumNb = 0;
    car.CarID = carId++;
    car.NbList = malloc(carsInSim * sizeof(short int));
    car.ITS = -1;
    car.IPos[0] = 0;
    car.IPos[1] = 0;
    car.PathLen = path.Length;

    return car;
}

void updateCells(Cell** cells, Car car, float R, float xMax, float yMax){
    float x, y;
    int index;
    x = car.PathXY[2 * car.Pos];
    y = car.PathXY[2 * car.Pos + 1];

    index = (y * xMax + x) / R;

    cells[index]->NCC++;
    if(cells[index]->NCC >= cells[index]->SCC){
        cells[index]->SCC += 10;
        if(!realloc(cells[index]->CellCars, cells[index]->SCC * sizeof(unsigned short))){
            fprintf(stderr, "Could not allocate memory!\n");
            exit(2);
        }
        cells[index]->CellCars[cells[index]->NCC] = car.CarID;
    }
}
