#pragma once //protects against multiple inclusions of this header file

#include <vector>
#include "graphics.h"

using namespace std;


//The Intersection class contains:
//The Intersection ID
//The vector of all the adjacent Intersection IDs
//The vector of all the Intersection Street Segments
class Intersection
{
    
public:
    //Data members
    int id;
    vector<unsigned> Adjacent_IntersectionIDs;
    
    //Constructors
    Intersection(int i, vector<unsigned> _Adjacent_IntersectionIDs);
};



