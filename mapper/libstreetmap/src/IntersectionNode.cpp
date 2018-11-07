#pragma once //protects against multiple inclusions of this header file
#include "IntersectionNode.h"



using namespace std;

t_point Convert_LatLon_to_XY(LatLon Point);

//Constructors
IntersectionNode::IntersectionNode(vector<AdjID_and_StSegID> _AdjIDs_and_SegIDs, int _IntersectionID)
{
    AdjIDs_and_SegIDs = _AdjIDs_and_SegIDs;
    IntersectionID = _IntersectionID;
    
    //Initialize as non-visited yet
    visited = 0;
    
    //Initialize as no path exists to this node yet
    Path_Exists_to_Node = 0;
    
    Pos = getIntersectionPosition( _IntersectionID );
    
    Position = Convert_LatLon_to_XY(  Pos  );
}