#pragma once

#include "LatLon.h"

//This structure is used in Hash_ClosestIntersection_2POI_Info
//It contains the Intersection ID of the closest intersection to this POI
//And the distance to the closest Intersection ID
//It contains the Latitude and Longitude of this Point of Interest
//Distance from the POI to our starting Intersection ID
struct ClosestIntersection_2POI
{
    // The member data. Data is public.
    int Closest_IntersectionID;
    double Dist_2_Closest_Intersection;
    LatLon LatLon_of_POI;
    double Dist_POI_2_StartID;
    

    
    // constructor
    ClosestIntersection_2POI( double _Dist_2_Closest_Intersection, int _Closest_IntersectionID)
    {
        Dist_2_Closest_Intersection = _Dist_2_Closest_Intersection;
        Closest_IntersectionID = _Closest_IntersectionID;
    }
    
    //Default Constructor
    //Only Initialize the distance to closest Intersection ID
    //Set Dist_2_Closest_Intersection initially as 900m
    //This is large enough to compare as we are trying to find the closest Intersection ID, we
    //will reset it
    ClosestIntersection_2POI( LatLon _LatLon_of_POI )
    {
        //Initial default value (Large Enough)
        Dist_2_Closest_Intersection = 900.0;
        
        LatLon_of_POI = _LatLon_of_POI;
    }
    
    
};









