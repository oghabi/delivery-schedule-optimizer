#pragma once //protects against multiple inclusions of this header file
#include "Feature.h"


using namespace std;


//Constructors
Feature::Feature(int _Num_Pts_Feature,bool _closed, vector<t_point> _XYs_of_Feature, string _Attribute_Key, string _Attribute_Value)
{
    Num_Pts_Feature = _Num_Pts_Feature;
    closed = _closed;
    XYs_of_Feature = _XYs_of_Feature;
    Attribute_Key = _Attribute_Key;
    Attribute_Value = _Attribute_Value;
}

