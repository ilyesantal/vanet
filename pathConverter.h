#include <math.h>

float* createPoints(Node *nodes, Segment segment, int *prevend, int *addedPoints, float *remainingFromPrevious, float *segmentLength, float *timeRequired)
{
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

    //How many points to add
    points = *addedPoints = (int) floor(length/speed[segment.type]);   //Also the time (in s) required for the segment
    time = *timeRequired = length/speed[segment.type];

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

float* convertPath(Path* path, Node* nodes, Segment* segments)
{
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
