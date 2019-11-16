#include "pathConverter.h"
#include <sys/types.h>
#include <stdlib.h>

Car getNextCar(int carsInSim, FILE** paths, Node* nodes, Segment* segments, int n)
{
    static int carId = 0;
    int i;
    Path path;
    Car car;

    char * line = NULL;
    size_t len = 0;

    int m = rand() % n + 1;


    for(i = 0; i < m; i++){
        getline(&line, &len, *paths);
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
