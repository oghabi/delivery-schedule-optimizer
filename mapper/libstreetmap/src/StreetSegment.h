#pragma once //protects against multiple inclusions of this header file

#include <vector>
#include <string>
#include "graphics.h"

#define KMPH_2_MperMIN 1000.0/60.0

using namespace std;


//The StreetSegment class contains:
//Is it a one-way segment?
//The street segment speed limit
//The street segment length
//The vector of the (x,y) coordinates of the start and end point of the street segment (and also the coordinates of the curve points, if there are any)
//The street name that this street segment belongs too
class StreetSegment
{
    
public:
    //Data members

    bool One_Way_Segment;
    float StreetSegment_SpeedLimit;
    double StreetSegmentLength;
    vector<t_point> XY_Coordinates_StreetSegmentPoints;
    string Name_of_StreetSeg;
    int StreetID_of_Segment;
    
    //The time from source
    //Trying to precompute it, because it will speed up Dijsktra's algorithm in M4
    double T_from_source;
    
    //Constructors
   StreetSegment(bool _One_Way_Segment, float SpeedLimit, double length, vector<t_point> XYCoordinates, string _Name_of_StreetSeg, int _StreetID_of_Segment );
};

