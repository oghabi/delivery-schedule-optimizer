#pragma once //protects against multiple inclusions of this header file

#include "AdjID_and_StSegID.h"
#include "graphics.h"
#include "LatLon.h"
#include "StreetsDatabaseAPI.h"
//#include "m1.h"
#include <vector>

using namespace std;


//This IntersectionNode class differs from "Intersection" class
//This class is used in milestone 3 to find the fastest path between two nodes



//The IntersectionNode class contains:
//The id of this Intersection
//The vector of all the adjacent Intersection IDs and its respective street segment IDs
//The Street Segment ID used to reach to this node
//A visited flag as we are inspecting the nodes, so we don't fall in an infinite loop
class IntersectionNode
{
    
public:
    //Data members
    vector<AdjID_and_StSegID> AdjIDs_and_SegIDs;
    int IntersectionID;
    int SegID_Reaching_This_Node;
    bool visited;
    double Travel_Time_from_Source;
    
    bool Path_Exists_to_Node;
    
    //(x,y) position
    t_point Position;
    
    LatLon Pos;
    
    // Constructors
    // SegID_Reaching_This_Node will be set in m3.cpp during the path-finding algorithm
    // Don't initialize it now
    IntersectionNode(vector<AdjID_and_StSegID> _AdjIDs_and_SegIDs, int _IntersectionID );
};





