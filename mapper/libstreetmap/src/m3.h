#pragma once

//m2.h in turn includes m1.h
//We want to access the hash tables made in m1.cpp
#include "m2.h"
#include "WaveElement.h"


//Convert Km/hr to m/s for speed limits
//it is KMPH to Meters per minute
#define KMPH_TO_METREperMIN 1000.0/60.0

/**********************************Gobal Variables***********************************/

//Use the vector of the path obtained to draw it out in m2.cpp in the draw_screen() function
//Just as a temporary to draw it on the map
extern vector<unsigned> The_Path_For_Drawing;

//Pointers to Intersection IDs we examined while we were searching for a path between 2 nodes
//We use this to clear the visited flags of the Nodes we examined in order for them to be searched
//in future path findings
//This is more efficient than looping through all the Intersection IDs and setting visited = 0
//NOTE: This vector is different from the path, it contains pointers to all nodes we examined but
//didn't include in the final path
extern vector<IntersectionNode*> Intersections_Examined_DuringSearch;






// Returns a path (route) between the start intersection and the end 
// intersection, if one exists. If no path exists, this routine returns 
// an empty (size == 0) vector. If more than one path exists, the path 
// with the shortest travel time is returned. The path is returned as a vector 
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end);


//Helper function for BFS (Breadth-First-Search)

bool BFSpath( IntersectionNode* SourceNode, int destID );

//Function to return the path found in a deque/vector
//It returns all the StreetSegment IDs that we have to travel through
deque<unsigned> BFS_Path_Traceback (int destID);

//Get the previous node
//As we are tracing "backwards" to find the path
int Get_Previous_Node ( int SegID, int CurrentNodeID );

// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can 
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street 
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
double compute_path_travel_time(const std::vector<unsigned>& path);

//Works with deque<unsigned>
double compute_path_travel_time_ofDeque(const std::deque<unsigned>& path);

// Returns the shortest travel time path (vector of street segments) from 
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned 
                   intersect_id_start, std::string point_of_interest_name);
