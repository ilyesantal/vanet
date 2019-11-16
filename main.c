#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "readFromFiles.h"
#include "createCar.h"

int main()
{
    int t, i, j, carsInSim, jmp, rnd, TMax;
    float xMax, xMin, yMax, yMin, R;

    char pth[20];

    Node* NODES = NULL;
    Segment* SEGMENTS = NULL;
    Car* CARS = NULL;

    FILE* paths;

    readInputs(&NODES, &SEGMENTS, &carsInSim, pth, &jmp, &rnd, &TMax, &xMax, &xMin, &yMax, &yMin, &R);

    srand(rnd);

    if( (paths = fopen(pth, "r")) == NULL){
            fprintf(stderr, "No such file!");
            exit(1);
    }

    CARS = malloc(sizeof(Car) * carsInSim);

    for(j = 0; j < carsInSim; j++){
        CARS[j] = getNextCar(carsInSim, &paths, NODES, SEGMENTS, jmp);
    }

    for(t = -500; t < TMax; t++){
        for(j = 0; j < carsInSim; j++){
            if(CARS[j].Pos == CARS[j].PathLen){
                CARS[j] = getNextCar(carsInSim, &paths, NODES, SEGMENTS, jmp);
            }
            CARS[j].Pos++;
        }

        if(t == 0){
            CARS[0].ITS=0;
            CARS[0].IPos[0]=CARS[0].PathXY[2*CARS[0].Pos];
            CARS[0].IPos[1]=CARS[0].PathXY[2*CARS[0].Pos+1];
        }
    }

    fclose(paths);

    free(NODES);
    free(SEGMENTS);

    exit(EXIT_SUCCESS);
}
