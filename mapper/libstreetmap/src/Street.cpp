#pragma once //protects against multiple inclusions of this header file
#include "Street.h"
#include <vector>

using namespace std;


//Constructors
Street::Street (int _id, double _length)
{
    id = _id;
    length = _length;
}

Street::Street ( vector<unsigned> _All_intersection_ID)
{
    All_intersection_IDs = _All_intersection_ID;
}

Street::Street ( vector<unsigned> _All_Street_Segment_IDs,  int any_int )
{
    All_Street_Segment_IDs = _All_Street_Segment_IDs;
}