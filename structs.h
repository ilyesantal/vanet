typedef struct {
    unsigned short Start; // ID of the node where the path is originated
    float Duration; // Duration of travel along the path in seconds (affected by speed) (needed only for statistics)
    float Length; // Length of the whole path in meters (needed only for statistics)
    unsigned short NSegm; // Number of road segments along the path
    unsigned short *SgmList; // Address of a dynamicaly allocated array storing the IDs of segments in the path
} Path;

typedef struct {
float x; // x coordinate of the node
float y; // y coordinate of the node
unsigned short NumSegm; // Number of connected road segments at the given node
unsigned short* SgmList; // List of IDs of connected segments
} Node;

typedef struct {
unsigned short N1; // One of the endpoints (node ID)
unsigned short N2; // Other endpoint (node ID)
unsigned short type; // Type of segment
} Segment;

typedef struct {
    unsigned short CarID; // ID of the vehicle (not the index)
    unsigned short PathLen; // Length of PathXY array in position units (2 floats)
    unsigned short Pos; // Index of PathXY to tell the actual position (x=PathXY[2*Pos], y=PathXY[2*Pos+1])
    float *PathXY; // List of positions (x-y) alog the path separated by 1 second
    short int ITS; // List of latest Information Time Stamps received from other vehicles (-1:invalid, no info) (méret>Nc)
    float IPos[2]; // The position of the agent, when it became informed (x-y coordinate)
} Car;

typedef struct cell{
    unsigned short NCC; // Number of cars in the cell
    unsigned short SCC; // Size of CellCars array
    unsigned short *CellCars; // List of indexes of cars in CARS array within the cell
} Cell;
