#include "traffic.h"

int main()
{
    int t, i, j, carsInSim, jmp, rnd, TMax, ncells;
    float xMax, yMax, R;

    char pth[20];

    Node* NODES = NULL;
    Segment* SEGMENTS = NULL;
    Car* CARS = NULL;
    Cell* CELLS = NULL;
    FILE* paths;

    readInputs(&NODES, &SEGMENTS, &carsInSim, pth, &jmp, &rnd, &TMax, &xMax, &yMax, &R);

    ncells = (xMax * yMax) / R;
    srand(rnd);

    CARS = malloc(sizeof(Car) * carsInSim);
    CELLS = malloc(sizeof(Cell) * ncells);

    for(i = 0; i < ncells; i++){
        CELLS[i].SCC = 10;
        CELLS[i].NCC = 0;
        CELLS[i].CellCars = malloc(sizeof(unsigned short) * CELLS[i].SCC);
    }

    if( (paths = fopen(pth, "r")) == NULL){
            fprintf(stderr, "No such file!");
            exit(1);
    }

    for(j = 0; j < carsInSim; j++){
        CARS[j] = getNextCar(carsInSim, &paths, NODES, SEGMENTS, jmp);
    }

    for(t = -500; t < TMax; t++){
        for(j = 0; j < carsInSim; j++){
            if(CARS[j].Pos == CARS[j].PathLen){
                CARS[j] = getNextCar(carsInSim, &paths, NODES, SEGMENTS, jmp);
            }
            updateCells(&CELLS, CARS[j], R, xMax, yMax);
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
