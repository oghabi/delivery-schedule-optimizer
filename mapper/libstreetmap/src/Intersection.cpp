#pragma once //protects against multiple inclusions of this header file
#include "Intersection.h"


using namespace std;



//Constructors
Intersection::Intersection(int i, vector<unsigned> _Adjacent_IntersectionIDs)
{
    id = i;
    Adjacent_IntersectionIDs = _Adjacent_IntersectionIDs;
}