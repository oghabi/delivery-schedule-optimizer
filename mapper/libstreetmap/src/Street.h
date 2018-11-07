#pragma once //protects against multiple inclusions of this header file

#include <vector>

using namespace std;


//Street Class contains:
//The Street ID
//The length
//The vector of all the intersection IDs that the street passes through
//The vector of all the street segment IDs of the Street
class Street
{
    
public:
    //Data members
    int id;
    double length;
    vector<unsigned> All_Street_Segment_IDs;
    vector<unsigned> All_intersection_IDs;
    
    //Constructors
    Street(int _id, double _length);  //1st Constructor
    Street ( vector<unsigned> _All_intersection_ID);  //2nd constructor
    Street ( vector<unsigned> _All_Street_Segment_IDs , int any_int); //3rd constructor
};

