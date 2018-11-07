#pragma once

#include <deque>

using namespace std;

//This structure is used in the hash table PreComputed_Paths
struct Path_and_Time
{
    // The member data. Data is public.
    deque<unsigned> Path;
    double PathTime;
    int to_Delivery;


    // constructor
    Path_and_Time( deque<unsigned> _Path,  double _PathTime, int _to_Delivery)
    {
        Path = _Path;
        PathTime = _PathTime;
        to_Delivery = _to_Delivery;
    }
    
};









