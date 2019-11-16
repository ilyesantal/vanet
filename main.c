#include <stdio.h>
#include <stdlib.h>
#include "structs.h"
#include "readFromFiles.h"
#include "createCar.h"

int main()
{
    int i, carsInSim, jmp, rnd;

    char pth[20];

    Node* NODES = NULL;
    Segment* SEGMENTS = NULL;
    Car* CARS = NULL;

    FILE* paths;

    readInputs(&NODES, &SEGMENTS, &carsInSim, pth, &jmp, &rnd);

    srand(rnd);

    if( (paths = fopen(pth, "r")) == NULL){
            fprintf(stderr, "No such file!");
            exit(1);
    }

    CARS = malloc(sizeof(Car) * carsInSim);

    //printf("%d\n%d\n", carsInSim, jmp);

    for(i = 0; i < carsInSim; i++){
        CARS[i] = getNextCar(carsInSim, &paths, NODES, SEGMENTS, jmp);
    }

    fclose(paths);

    free(NODES);
    free(SEGMENTS);

    exit(EXIT_SUCCESS);
}
