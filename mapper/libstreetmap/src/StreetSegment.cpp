#pragma once //protects against multiple inclusions of this header file
#include "StreetSegment.h"

using namespace std;



StreetSegment::StreetSegment( bool _One_Way_Segment, float SpeedLimit, double length, vector<t_point> XYCoordinates, string _Name_of_StreetSeg, int _StreetID_of_Segment )
{
    One_Way_Segment = _One_Way_Segment;
    StreetSegment_SpeedLimit = SpeedLimit;
    StreetSegmentLength = length;
    XY_Coordinates_StreetSegmentPoints = XYCoordinates;
    Name_of_StreetSeg = _Name_of_StreetSeg;
    StreetID_of_Segment = _StreetID_of_Segment;
    
    T_from_source = StreetSegmentLength / ( StreetSegment_SpeedLimit * KMPH_2_MperMIN );
}
