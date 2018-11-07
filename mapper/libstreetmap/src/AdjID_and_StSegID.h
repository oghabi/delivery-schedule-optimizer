#pragma once



//This structure is used in the IntersectionNode class
//It contains the adjacent ID of an Intersection and its respective street segment ID
struct AdjID_and_StSegID
{
    // The member data. Data is public.
    int AdjID;
    int StSegID;

    // Handy constructor
    AdjID_and_StSegID(int _AdjID, int _StSegID)
    {
        AdjID = _AdjID;
        StSegID = _StSegID;
    }
    
};








