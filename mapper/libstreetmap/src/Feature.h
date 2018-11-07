#pragma once //protects against multiple inclusions of this header file

#include <vector>
#include <string>
#include "graphics.h"

using namespace std;


//Feature Class contains aspects of a Feature Attribute:
//Is it closed polygon? If not then it is a multi-segment line.
//Vector<t_point> of all the (x,y) coordinates of that Feature
//The Attribute Key of that Feature
//The Attribute return value of that Feature
class Feature
{
    
public:
    //Data members
    
    int Num_Pts_Feature;
    //Is it a closed shaped for drawing
    //Check the first and last LatLon points
    bool closed;
    vector<t_point> XYs_of_Feature;
    string Attribute_Key;
    string Attribute_Value;
    
    //Constructors
    Feature(int _Num_Pts_Feature ,bool _closed, vector<t_point> _XYs_of_Feature, string _Attribute_Key, string _Attribute_Value);  //1st Constructor
   
};

