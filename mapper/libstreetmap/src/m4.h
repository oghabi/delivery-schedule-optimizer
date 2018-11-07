#pragma once

#include "m3.h"


    // This routine takes in a vector of N intersections that must be
    // traversed in a path, and another vector of M intersections that 
    // are legal start and end points for the path. The first vector 
    // gives the intersection ids of a set of locations at which 
    // deliveries must be made. The second vector gives the intersection
    // ids of courier company depots containing trucks; you start at any
    // one of these depots and end at any one of the depots.
    // You can assume that N is always at least 1, and M is always at 
    // least one (i.e. both input vectors are non-empty).  You can also
    // assume that no intersection is repeated in either list and that
    // no intersection is both a delivery location and a depot (i.e. no
    // intersection will appear in both vectors).
    // This routine returns a vector of street segment ids that form a 
    // path, where the first street segment id is connected to a depot
    // intersection, and the last street segment id also connected to a
    // depot intersection.  If no path connecting all the delivery locations
    // and a start and end depot exists, this routine should return an 
    // empty (size == 0) vector.

    std::vector<unsigned> traveling_salesman(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations);


    //Dijkstra's algorithm to expand from one intersection to all the other 49 intersections
    //And leave bread crumbs to trace the path back
    //We call this 50 times to precompute all the paths and store them
    //Parameters are a pointer to the starting IntersectionNode object in Hash_IntersectionNode_Info and the vector of the 50 intersection IDs
    //Pass the intersections to traverse by reference to not keep on copying the vector
    void Dijkstra_M4( IntersectionNode* SourceNode, vector<unsigned>& intersections_to_traverse  );


    //This function finds the time of the path given a vector<unsigned> of delivery IDs
    //Pass the vector by reference to avoid copying the vector
    //Use this function mainly to compute whole delivery time when we do K-opt
    double FindTime_FromDeliveryList ( vector<unsigned>& DeliveryIDs  );


    //This is the 2-opt algorithm
    //Pass in the vector of delivery IDs and the current Best_Path_Time
    //Pass BOTH by reference, in case we find a better solution
    //Returns one if a better solution was found
    bool K_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    void Three_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    void Four_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    //Returns one if a better solution was found
    //Also pass in the time so we dont exceed our time limit
    bool Two_Point_Swap ( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    bool Oghab_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    bool SwapLeftRight_Two( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    void AllPermutations( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );

    void Wiki_2_Opt( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime );






