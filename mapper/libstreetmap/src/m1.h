#pragma once //protects against multiple inclusions of this header file

#include <string>
#include <vector>
#include <deque>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <list>
#include <queue>
#include <cmath>
#include <time.h>
#include <stdlib.h> //For Rand() function
#include "StreetsDatabaseAPI.h"
#include "graphics.h"

//The Added Classes
#include "Street.h"
#include "Intersection.h"
#include "StreetSegment.h"
#include "Feature.h"
#include "IntersectionNode.h"
#include "ClosestIntersection_2POI.h"
#include "Path_and_Time.h"

//Readline open source library for parsing commands by the user
#include <readline/readline.h>
#include <readline/history.h>


using namespace std;

//In order to use the data in the hash table to draw the map in the file m2.cpp
//We need to include it in the header file
extern std::unordered_map<int,StreetSegment> Hash_StreetSegment_Info;
extern std::unordered_map<int,Feature> Hash_Feature_Info;
extern std::unordered_map<string,Street> Hash_Street_Info;
extern std::unordered_map<string,Intersection> Hash_Intersection_Info;

//To set the intial coordinates of the visible map based on the Maximum and Minimum LatLon point of each map
//In order to be compatible with any maps they give us in the auto-tester
extern LatLon MaximumLatLon_ofMap;
extern LatLon MinimumLatLon_ofMap;

//The hash tables to be used in m3.cpp (used to find the fastest path between 2 nodes)
extern std::unordered_map<int,IntersectionNode> Hash_IntersectionNode_Info;

//These hash tables are used in m3.cpp to find the path from an Intersection ID to a POI name entered
extern std::unordered_map< string,vector<unsigned> > Hash_POI_IDs;
extern std::unordered_map< unsigned, ClosestIntersection_2POI> Hash_ClosestIntersection_2POI_Info;


extern vector<unsigned> StreetSegIDs_Highway;
extern vector<unsigned> StreetSegIDs_Zoom3;
extern vector<unsigned> StreetSegIDs_Zoom2_Grey;


extern vector<const char*> Intersection_Names_AutoCompletion;


//Global Data Structure for Milestone 4
//Precomputing all the paths with Dijkstra
extern std::unordered_map<int, std::unordered_map<int, Path_and_Time>  > PreComputed_Paths;

//use these defines whenever you need earth radius 
//or conversion from degrees to radians
#define EARTH_RADIUS_IN_METERS 6372797.560856
#define DEG_TO_RAD 0.017453292519943295769236907684886
#define pi           3.14159265358979323846  /* pi */

#define TIME_LIMIT 30 //M4 time limit

//function to load bin or osm map
bool load_map(std::string map_name);

//close the loaded map
void close_map();

//function to return intersection id for an intersection name
int find_intersection_id_from_name(std::string intersection_name);

//function to return street id for an intersection name
unsigned find_street_id_from_name(std::string street_name);

//function to return the street segments for a given intersection 
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name);
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id);

//function to return street names at an intersection
std::vector<std::string> find_intersection_street_names(std::string intersection_name);
//Write a Unit Test for this
std::vector<std::string> find_intersection_street_names(unsigned intersection_id);

//can you get from intersection1 to intersection2 using a single street segment (hint: check for 1-way streets too)
//They have to be connected by ONE street segment, not continuous street segments
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2);

//find all intersections connected by one street segment from given intersection (hint: check for 1-way streets too)
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name);
void test();

//for a given street, return all the street segments
std::vector<unsigned> find_street_street_segments(std::string street_name);

//for a given street, find all the intersections
std::vector<unsigned> find_all_street_intersections(std::string street_name);

//find distance between two coordinates
double find_distance_between_two_points(LatLon point1, LatLon point2);

//find the length of a given street segments (in meters)
double find_street_segment_length(unsigned street_segment_id);

//find the length of a whole street
 double find_street_length(std::string street_name);

//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
double find_segment_travel_time(unsigned street_segment_id);

//find the nearest point of interest (by name) to a given position
std::string find_closest_point_of_interest(LatLon my_position);



//Helper functions used in load_map()


double Calculate_street_length(std::string street_name);

//Objective is to have a hash table with key = street names and each street name having a value containing a vector of all the intersection IDs the street goes through
void All_IntersectionIDs_for_Street(unsigned IntersectionID);

//Returns vector of adjacent intersections to be stored in Hash_Intersection_Info
//BUT it also takes in a vector by reference
//It has to fill up this vector by the adjacent Intersection IDs AND their respective street segment IDs
//This second vector will be stored in Hash_IntersectionNode_Info
vector<unsigned> adjacent_intersections(unsigned intersection_ID, vector<AdjID_and_StSegID>& AdjIDs_and_SegIDs);


//Converts a LatLon point to the XY coordinate system and returns a t_point struct
t_point Convert_LatLon_to_XY(LatLon Point);

//Converts an XY coordinate and to a LatLon point
LatLon Convert_XY_to_LatLon(float x, float y);

//Loads a vector<t_point> of all the (x,y) coordinates of the points of the street segment (including the .from and .to points and all the curve points)
//This function also computes the street segment length at the same time for efficiency
//Both parameters are passed in by reference
void Coordinates_of_StreetSegment_Points_AND_ComputeStSegLength(vector<t_point>& VectorXYCoordinates , int StreetSegmentID, double& StreetSegment_Length);


//Helper functions to store Feature objects in Hash_Feature_Info

//Is the Feature ID inputted closed?
bool Is_It_Closed (int Feature_ID, int Num_Points_of_Feature);

//Get all the (x,y) coordinates of the feature in a vector
vector<t_point> getXY_of_Feature (int Feature_ID, int Num_Points_of_Feature);

void Clear_Global_Variables();



