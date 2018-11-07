#pragma once

#include "IntersectionNode.h"

//This is used to say that we reached this node with no edge (street segment)
//In order to say that this is our starting node
#define NO_EDGE -1

//This structure is used in m3.cpp, in the BFS helper function
//WaveElement is an element we have to inspect as we are searching for the Destination ID
//This one element contains:
//A pointer to the IntersectionNode (object) to be inspected
//The street segment ID used to reach the node to be inspected
struct WaveElement
{
    // The member data. Data is public.
    IntersectionNode* Node_ptr;
    int SegID_reaching_this_node;
    
    //This is g(t) = Dijkstra's (Time from intial node until current node)
    //This is an EXACT value
    double Travel_Time;
    
    //This is f(t) = h(t) + g(t)
    double Heuristic_And_TimeFromInitial;
    
    //_Time_2_Final_Dest:
    //This will be our heuristic- h(t) = Time from current node until final destination node
    //This is an ESTIMATE
    //The closer the estimate to the real value, the faster the program

    WaveElement( IntersectionNode* _Node_ptr , int _SegID_reaching_this_node, double _Travel_Time, double _Time_2_Final_Dest)
    {
        Node_ptr = _Node_ptr;
        SegID_reaching_this_node = _SegID_reaching_this_node;
        Travel_Time = _Travel_Time;
        
        
        Heuristic_And_TimeFromInitial = (_Travel_Time + _Time_2_Final_Dest) ;
    }
    
    WaveElement( IntersectionNode* _Node_ptr , int _SegID_reaching_this_node, double _Travel_Time)
    {
        Node_ptr = _Node_ptr;
        SegID_reaching_this_node = _SegID_reaching_this_node;
        Travel_Time = _Travel_Time;
        
    }
    
    
    //Define operator < in order to store the WaveElements in a heap
    //This will be part of our struct comparator
    bool operator<( WaveElement& rhs )
    {
        if ( this->Heuristic_And_TimeFromInitial < rhs.Heuristic_And_TimeFromInitial ) return 0;
        //if ( this->Travel_Time < rhs.Travel_Time ) return 0;
        else return 1;
    }
    
    
};




struct comparator_WaveElement
{
    bool operator() ( WaveElement i, WaveElement j) {
        return i < j; }
};





