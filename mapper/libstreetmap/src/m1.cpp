#pragma once //protects against multiple inclusions of this header file
#include "m1.h"
#include "m2.h"
#include "m3.h"
#include <iostream>
#include <algorithm>



using namespace std;

//Check the files Street.h Intersection.h and StreetSegment.h to se the definitions of the added Classes


//The Data structures used in load_map()
/***********************************Data Structures**********************************************/

//A Hash table with key = Street name and value = Street object
std::unordered_map<string,Street> Hash_Street_Info;

//A Hash table with key = Intersection name and value = Intersection object
std::unordered_map<string,Intersection> Hash_Intersection_Info;

//A Hash table with key = Street Segment ID and value = StreetSegment object
std::unordered_map<int,StreetSegment> Hash_StreetSegment_Info;

//A Hash table with key = Feature ID and value = Feature object
std::unordered_map<int,Feature> Hash_Feature_Info;

//A Hash table with key = Intersection ID and value = IntersectionNode object
std::unordered_map<int,IntersectionNode> Hash_IntersectionNode_Info;

//A Hash table with key = POI Name and value = A vector containing all the POI IDs of this name
//Ex: Tim Hortons is a POI name that has 244 POI IDs at different locations
std::unordered_map< string,vector<unsigned> > Hash_POI_IDs;

//A Hash table with key = POI ID and value = ClosestIntersection_2POI object containing information
//about the closest Intersection ID to a POI ID
std::unordered_map< unsigned, ClosestIntersection_2POI> Hash_ClosestIntersection_2POI_Info;
/************************************************************************************************/

/********************************Data Structures FOR GRAPHICS***********************************/
//These are vectors containing street segment IDs based on different categories (Speed Limit/Length)

vector<unsigned> StreetSegIDs_Highway;

vector<unsigned> StreetSegIDs_Zoom3;

vector<unsigned> StreetSegIDs_Zoom2_Grey;

vector<const char*> Intersection_Names_AutoCompletion;

/************************************************************************************************/


/***********************************Global Variables**********************************************/
//These variables are used to find the Average Latitude of the entered map in order to draw a graph of the map
double MapAverageLatitude, MapMinimumLatitude, MapMaximumLatitude;


//Use these variables to set_visible_world() for drawing the map
//Currently only the values to display the Toronto Map is hard coded in
//So other maps don't have the correct intial coordinates and are zoomed out way too much
LatLon MaximumLatLon_ofMap;
LatLon MinimumLatLon_ofMap;


/************************************************************************************************/


//load the map
bool load_map(std::string map_name) {

    bool load_success = loadStreetDatabaseBIN(map_name);
    
    //Creating Data Structures
    
    //Initialize Minimum and Maximum Latitudes to a value so that we can compare it to other values
    MapMinimumLatitude = ( getIntersectionPosition(0) ).lat;
    MapMaximumLatitude = ( getIntersectionPosition(0) ).lat;
    
    
    //Initialize Minimum and Maximum LatLon of the map so we can compare it to other values
    
    MinimumLatLon_ofMap.lat = getIntersectionPosition(0).lat;
    MinimumLatLon_ofMap.lon = getIntersectionPosition(0).lon;

    MaximumLatLon_ofMap.lat = getIntersectionPosition(0).lat;
    MaximumLatLon_ofMap.lon = getIntersectionPosition(0).lon;
    
    
    
    //Go through all the Points of Interest
    //Here we will be storing POI names and their associated POI IDs at different locations
    //In Hash_POI_IDs
    //Ex: Tim Hortons is a POI name that has 244 POI IDs at different locations
//    for (int i = 0; i < getNumberOfPointsOfInterest() ; i++)
//    {
//        
//        //Get the latitude and longitude of the POI ID
//        LatLon Temp = getPointOfInterestPosition( i );
//        
//        //Start intializing objects in Hash_ClosestIntersection_2POI_Info to save time
//        //When going through all the Intersection IDs
//        Hash_ClosestIntersection_2POI_Info.insert( make_pair(i, ClosestIntersection_2POI(Temp) ) );
//        
//        //Filling in Hash_POI_IDs
//        
//        string POI_name = getPointOfInterestName( i );
//        
//        //Search if we already inserted this POI name
//        unordered_map< string,vector<unsigned> >::iterator iter = Hash_POI_IDs.find( POI_name );
//        
//        //If we didn't insert it then insert it, then insert it as a new element
//        if ( iter == Hash_POI_IDs.end() )
//        {
//            //Store the POI ID in a vector
//            vector<unsigned> temp;
//            temp.push_back( i );
//            
//            //Insert the POI name and its ID in Hash_POI_IDs
//            Hash_POI_IDs.insert( make_pair( POI_name, temp ) );
//        }
//        
//        //We already inserted this POI name
//        //This means it is the same POI name but at a different location in the map
//        //Add this POI ID to the vector
//        else
//        {
//            (iter->second).push_back( i );
//        }
//        
//    }//End going through POIs
    
    
    
    //Going through all the Intersection IDs
    for (int i = 0; i < getNumberOfIntersections(); i++)
    {
        
        //Get the LatLon of this Intersection ID
        LatLon Inter_LatLon = getIntersectionPosition( i );
//        
//        //Find closest Intersection ID to each POI ID stored in Hash_ClosestIntersection_2POI_Info
//        for (std::unordered_map< unsigned, ClosestIntersection_2POI>::iterator iter2 = Hash_ClosestIntersection_2POI_Info.begin();
//             iter2 != Hash_ClosestIntersection_2POI_Info.end();
//             iter2++)
//        
//        {
//            //Find the distance between Intersection ID and the POI ID
//            double Dist_Inter2POI = find_distance_between_two_points(Inter_LatLon, (iter2->second).LatLon_of_POI  );
//            
//            //Check if this distance found is < previously saved closest distance in Hash_ClosestIntersection_2POI_Info (initially 900m)
//            
//            //NOTE:
//            //Usually the closest Intersection ID to each POI ID is less than 900m
//            //So only consider updating the hash table members if the distance found between the Intersection ID & POI ID is < 900m
//            
//            //If its less, then this Intersection ID is now considered to be the closest Interesection to the POI ID
//            //Update the memebers in Hash_ClosestIntersection_2POI_Info
//            if ( Dist_Inter2POI < (iter2->second).Dist_2_Closest_Intersection  )
//            {
//                //Update closest Intersection ID
//                (iter2->second).Closest_IntersectionID = i;
//                
//                //Update the distance until the closest Intersection ID
//                (iter2->second).Dist_2_Closest_Intersection = Dist_Inter2POI;
//            }
//            
//            //If the distance found is more than the previously saved closest distance, then do nothing
//        }
        
        
        
        
        //Get the information for the data members of an Intersection object
        string IntersectionName = getIntersectionName(i);
        
        //This vector contains struct of the adjacent IDs and their respective street segment IDs
        //It will be stored in Hash_IntersectionNode_Info
        vector<AdjID_and_StSegID> AdjIDs_and_SegIDs;
        
        //The vector "AdjIDs_and_SegIDs" is passed in by reference to modify the original vector itself
        vector<unsigned> AdjIDs = adjacent_intersections(i, AdjIDs_and_SegIDs);
    
        
        //Converts a string to a char*
        const char* String_2_Char = IntersectionName.c_str();
        
        Intersection_Names_AutoCompletion.push_back(String_2_Char);
        
        
        Intersection temp(i, AdjIDs);
        //Insert the object into the Hash table
        Hash_Intersection_Info.insert( make_pair(IntersectionName,temp) );
        
        
        
        // Insert data in Hash_IntersectionNode_Info
        // Key = Intersection ID and value = IntersectionNode object
         IntersectionNode temp1( AdjIDs_and_SegIDs, i );
         Hash_IntersectionNode_Info.insert( make_pair( i ,temp1) );
        
        
        
        //This function takes an Intersection ID as a parameter
        //Finds the Street names of the Intersection ID using another function
        //Adds this Intersection ID to Hash_Street_Info
        //Specifically, it adds it to Street objects which have keys == to the Street names of this Intersection ID
        All_IntersectionIDs_for_Street(i);
        
        
        
        //As we are going through all the intersections, try to find the minimum and maximum latitude of this map
        //This is done by retrieving the LatLon position of each intersection using getIntersectionPosition()
        //Compare these values to the previous maximum and minimum latitudes and update the previous values accordingly if it is either smaller or larger than them.
        
        //The new coordinate to be compared with previous saved latitudes
        LatLon NewCoordinate = getIntersectionPosition(i);
        
        
        //Do the comparison
        if ( NewCoordinate.lat <  MapMinimumLatitude)  MapMinimumLatitude = NewCoordinate.lat;
        else if ( NewCoordinate.lat >  MapMaximumLatitude) MapMaximumLatitude = NewCoordinate.lat;

        
        //Find the Maximum and Minimum LatLon Points of the map inserted
    if ( NewCoordinate.lat < MinimumLatLon_ofMap.lat && NewCoordinate.lon < MinimumLatLon_ofMap.lon )
    {
        MinimumLatLon_ofMap.lat = NewCoordinate.lat;
        MinimumLatLon_ofMap.lon = NewCoordinate.lon;
    }
    
else if (NewCoordinate.lat > MaximumLatLon_ofMap.lat && NewCoordinate.lon > MaximumLatLon_ofMap.lon )
    {
        MaximumLatLon_ofMap.lat = NewCoordinate.lat;
        MaximumLatLon_ofMap.lon = NewCoordinate.lon;
    }
        
       
        
    }
    
    
    //Now that we have figured out the exact values of the minimum and maximum latitudes
    //Update the MapAverageLatitude
    MapAverageLatitude = (MapMinimumLatitude + MapMaximumLatitude)/2;
    

    
    
    //Going through all the Street segments
    //Used to find the street segment IDs for a street
    for (int i = 0; i < getNumberOfStreetSegments(); i++)
    {
        
        //Get the Street name that this Street segment belongs to
        string St_name = getStreetName( getStreetSegmentStreetID(i) );
        
        //Search Hash_Street_Info to see if this Street name (key) already exists
        unordered_map<string,Street>::iterator iter = Hash_Street_Info.find( St_name );
        
        //Key exists (it has been inserted before)
        if ( iter != Hash_Street_Info.end() )
        {
            //Just add this Street Segment ID to the vector containing all the StreetSegment IDs of this Street
            ((iter->second).All_Street_Segment_IDs).push_back( i );
        }
        
        //Key doesn't exist
        else
        {
            //Add the street segment ID to a vector and add that vector to Hash_Street_Info
            vector<unsigned> intial_addition = {i};
            Street temp(intial_addition, 5);
            
            //Key is the Street name
            Hash_Street_Info.insert( make_pair(St_name ,temp ) );
        }
        
        
        
        //Now loading StreetSegment Object into Hash_StreetSegment_Info
        
        double SpeedLimit = getStreetSegmentSpeedLimit(i);
        
        //Load the vector with the (x,y) coordinates of all the points of this street segment
        //Note: the XY_Coordinates vector is only used to draw the map
        //You cannot use it to compute the distance between 2 points as the Average Latitude is different
        vector<t_point> XY_Coordinates;
        //Two objectives in one statement
        //This line loads the vector with the (x,y) coordinates of all points of the street segment
        //AND updates the street segment length at the same time
        double SegmentLength;
        Coordinates_of_StreetSegment_Points_AND_ComputeStSegLength( XY_Coordinates,i, SegmentLength);
        
        //The street name that this streetsegment belongs to
        //Bunch of steps in one line
        //Get the Street ID that this street segment belongs to
        //Convert the Street ID to the Street name
        string St_Name_of_SreetSeg =   getStreetName( getStreetSegmentStreetID( i ) );
        
        //If one way segment then it is 1, else 0
        bool One_Way_Segment =  getStreetSegmentOneWay ( i );
        
        int StID_of_Segment = getStreetSegmentStreetID( i );
        
        
        //Creating a temporary object to insert in the hash table Hash_StreetSegment_Info
         StreetSegment temp( One_Way_Segment, SpeedLimit, SegmentLength, XY_Coordinates , St_Name_of_SreetSeg, StID_of_Segment );
         Hash_StreetSegment_Info.insert( make_pair(i, temp  )  );
        
        
        
    }
    
    
    //Going through all street IDs
    for (int i = 0; i < getNumberOfStreets(); i++)
   {
        //Get the information for the data members of a Street object
        string name = getStreetName(i);
        //Calculate_street_length() is a helper function
        //It stores all the street segments of a street in a vector
        //And sums of the individual street segment lengths in order to get the whole street length
        double St_length = Calculate_street_length( name );
        Street temp(i, St_length );

        //Search Hash_Street_Info to see if there already exists a key with this Street name
        unordered_map<string,Street>::iterator iter = Hash_Street_Info.find(name);
       
       //Key exists
       if (iter != Hash_Street_Info.end() )
        {
            //Update the data members of the Street Object of that key to the correct values
            (iter->second).length = St_length;
            (iter->second).id = i;
        }

       //Key doesn't exist yet
        else
            //Insert this new Street object value with key = street name
            Hash_Street_Info.insert( make_pair(getStreetName(i) , temp ) );

     }

    
    //Loop through Hash_StreetSegmetns_Info to fill up the vectors for drawing the streets faster
    for (std::unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.begin();
         iter != Hash_StreetSegment_Info.end();
         iter++)
    {
        //Get the Street name this Street Segment belongs to
        string StreetSeg_Name = (iter->second).Name_of_StreetSeg;

        unordered_map<string,Street>::iterator iter2 = Hash_Street_Info.find( StreetSeg_Name );
        
        //It is a major street
        //Make sure, it is not unknown
        //Because all unknown streets are clumped into one category : (unknown)
        if ( (iter2->second).length > 5000 && StreetSeg_Name != "(unknown)")
        {
            if ( (iter->second).StreetSegment_SpeedLimit > 80 )
            {
                //Add the street segment ID to this vector
                StreetSegIDs_Highway.push_back( iter->first );
            }
            
            //else the speed limit was < 80
            else
            {
                StreetSegIDs_Zoom3.push_back( iter->first );
            }
        }
        
        //If this street segment belongs to a street < 5000m
        //Draw it in light grey with the thinnest line
        else
        {
            StreetSegIDs_Zoom2_Grey.push_back( iter->first );
        }
        
        
//        if ( (iter2->second).length > 2000 && StreetSeg_Name != "(unknown)" )
//            //Draw it at this zoom level
//        {
//             if ( (iter->second).StreetSegment_SpeedLimit < 80 )
//                 StreetSegIDs_Zoom1_ThickStreets.push_back( iter->first );
//        }
//        
//        else
//        {
//            StreetSegIDs_Zoom1_ThinStreets.push_back( iter->first );
//        }
        
    }
    
    
    
    
    
    
    //All the major attribute keys in Toronto
    vector<string> All_Attribute_Keys =
    {
        "waterway", //6010 Attributes in T.O.  //Keep
        "natural", //4610  //Keep
        "leisure", //2633  //Keep
        "tunnel", //1265   //Keep
        "water", //48      //keep
        "land", //33
        "landuse", //25
        "place" //12
    };
    
    
    //Go through all the Features in the map and cache them in an unordered map
    //There are about 14,000 points in Toronto
    for (int i = 0; i < getFeatureCount(); i++)
    {
        //The number of (LatLon) points in a feature
       int Num_Pts_of_Feature = getFeaturePointCount( i );
        
        //Is the Feature a closed shape?
        bool isClosed = Is_It_Closed( i, Num_Pts_of_Feature );
        
        //Get all the (x,y) coordinates of the feature in a vector
        //Use it later for drawing them
        vector<t_point> XY_of_Feature = getXY_of_Feature ( i, Num_Pts_of_Feature );
        
        
        //The Attribute Key and return value of that feature
        string Attribute_Key, Attribute_Value;
        
        
        //Go through all the above attributes and see what attribute this feature belongs to
        //As soon as we find the attribute, break out of the loop to save time
        for (int j = 0; j < All_Attribute_Keys.size() ; j++)
        {
            //Temp string to check which attribute key belongs to this feature
            //If it doesn't belong, temp (Attribute value) will be an empty string ""
            //If it does belong, temp will not be empty
            string temp = getFeatureAttribute(i, All_Attribute_Keys[j]  );
            
            //If the Attribute key is correct and matches this feature
            if (temp != "")
            {
                Attribute_Key = All_Attribute_Keys[j];
                Attribute_Value = temp;
                //Commenting line below fixes the issue with finding the attribute "tunnel"
                //and showing it on the map with dashed lines
               // goto Found_Attribute;
            }
            
        }
        
        
    Found_Attribute:
        
        //Create a temporary object to insert it into the hash table
        Feature temp(Num_Pts_of_Feature, isClosed, XY_of_Feature, Attribute_Key, Attribute_Value   );
        Hash_Feature_Info.insert( make_pair(i , temp )  );
    }
    
    
    return load_success;
}


//close the map
void close_map()
{
    closeStreetDatabase();
}


//implementing the rest of the functions provided in m1.h


//function to return intersection id for an intersection name
//Search Hash_Intersection_Info to see if a key with the entered Intersection name exists
//If it exists then return the Intersection ID data member from the corresponding key value (the Intersection object)
//If it doesn't exist, then invalid intersection name. Print appropriate output
int find_intersection_id_from_name(std::string intersection_name)
{
    
    //Using iterators, search hash table to see if this key exists
  unordered_map<string,Intersection>::iterator iter = Hash_Intersection_Info.find( intersection_name );
    
    //Error Handling
    if ( iter == Hash_Intersection_Info.end() )   
    {
        //By our convention, lets put this # to indicate that we are looking for isn't found
        return -666;
    }
        
    else return ( (iter->second).id );
    
}


//function to return street id for a street name
//Search Hash_Street_Info to see if a key with this street name exists
//If it exists then return the corresponding street ID data member from the Street object returned
//If it doesn't exist, print error message
unsigned find_street_id_from_name(std::string street_name)
{
    //Use iterators to search the hasht table
    unordered_map<string,Street>::iterator iter = Hash_Street_Info.find(street_name);
    
    if (iter != Hash_Street_Info.end() )
    
     return( ((iter->second).id) );
    
}



//function to return the street segments for a given intersection name
//Note: street segments have no name, only unique ID
std::vector<unsigned> find_intersection_street_segments(std::string intersection_name)
{
    
    //The ID of the intersection
    unsigned int Intersection_ID = find_intersection_id_from_name (intersection_name);
    
    //Create the vector which containts the Street segments IDs for a given intersection
    std::vector<unsigned> street_segments_IDs;
    
    //Number of street segments at the Intersection
    unsigned int Num_StreetSeg_at_Intersection = getIntersectionStreetSegmentCount(Intersection_ID);
    
    for (int i = 0; i < Num_StreetSeg_at_Intersection ; i++)
    {
        street_segments_IDs.push_back( getIntersectionStreetSegment(Intersection_ID, i)  );
    }
    
    
    return street_segments_IDs;
    
}

//function to return the street segments for a given intersection ID
std::vector<unsigned> find_intersection_street_segments(unsigned intersection_id)
{
    
    //Create the vector which containts the Street segments IDs for a given intersection
    std::vector<unsigned> street_segments_IDs;
    
    //Number of street segments at the Intersection
    unsigned int Num_StreetSeg_at_Intersection = getIntersectionStreetSegmentCount(intersection_id);
    
    for (int i = 0; i < Num_StreetSeg_at_Intersection ; i++)
    {
        street_segments_IDs.push_back( getIntersectionStreetSegment(intersection_id, i)  );
    }
    
    
    return street_segments_IDs;
}


//function to return street names at an intersection from an Intersection name
//First use existing function to get a vector of the street segment IDs at that intersection
//Second use getStreetSegmentStreetID() to get the ID of the street to which this street segment belongs
//Create a new vector to store the street IDs
//Finally create another string vector and use getStreetName() to extract the names of the streets from their IDs and return a vector containing all the names
std::vector<std::string> find_intersection_street_names(std::string intersection_name)
{
    //Store the street segment IDs at intersection
    std::vector<unsigned> street_seg_IDs = find_intersection_street_segments( intersection_name );
    
    //To store the street IDs which are associated to the above street segment IDs
    std::vector<unsigned> street_IDs;
    
    //Vector to store the names of the streets at the intersection
    std::vector<std::string> StreetName_at_Intersection;
    
    //Combine both lines below in one loop to speed up the process
    for (int i = 0; i < street_seg_IDs.size(); i++)
    {
        //store street IDs associated to the corresponding street segment ID
        street_IDs.push_back(  getStreetSegmentStreetID( street_seg_IDs[i] )  );
        
        //Check if we have not already entered this street name
        //Because (usually) 2 Street segments in an intersection have the same Street ID
        //Therefore the names get added twice
        std::vector<std::string>::iterator iter = find( StreetName_at_Intersection.begin(), StreetName_at_Intersection.end(), getStreetName(street_IDs[i]) );
        
        
        //This means that we have not found this street name and it is not repetitive
        if ( iter == StreetName_at_Intersection.end() )
        //Store street names by extracting it from their street IDs
        StreetName_at_Intersection.push_back( getStreetName(street_IDs[i]) );
    }
    
    return StreetName_at_Intersection;
}

//function to return street names at an intersection from the intersection ID
//Same steps as function above
std::vector<std::string> find_intersection_street_names(unsigned intersection_id)
{
    std::vector<unsigned> street_seg_IDs = find_intersection_street_segments( intersection_id );
    
    //To store the street IDs which are associated to the above street segment IDs
    std::vector<unsigned> street_IDs;
    
    //Vector to store the names of the streets at the intersection
    std::vector<std::string> StreetName_at_Intersection;

    //Combine both both lines below in one loop to speed up the process
    for (int i = 0; i < street_seg_IDs.size(); i++)
    {
        //store street IDs associated to the corresponding street segment ID
        street_IDs.push_back(  getStreetSegmentStreetID( street_seg_IDs[i] )  );
        
        //Check if we have not already entered this street name
        //Because (usually) 2 Street segments in an intersection have the same Street ID
        //Therefore the names get added twice
        std::vector<std::string>::iterator iter = find( StreetName_at_Intersection.begin(), StreetName_at_Intersection.end(), getStreetName(street_IDs[i]) );
        
        
        //This means that we have not found this street name and it is not repetitive
        if ( iter == StreetName_at_Intersection.end() )
            //Store street names by extracting it from their street IDs
            StreetName_at_Intersection.push_back( getStreetName(street_IDs[i]) );
    }
    
    return StreetName_at_Intersection;
    
}


//Can you get from intersection1 to intersection2 using a SINGLE street segment (hint: check for 1-way streets too)
//First get a vector of the adjacent Intersection IDs of Intersection one
//This info is stored in the hash table in load_map() : Hash_Intersection_Info
//Now check to see if the ID of intersection 2 is in this vector
//In order for them to be connected by one street segment, this ID should be inside that vector
//If found, return true since that means that intersection 1 and 2 are directly connected
//else return false
bool are_directly_connected(std::string intersection_name1, std::string intersection_name2)
{
    //If user enters same intersection
    if (intersection_name1 == intersection_name2) return 1;
    
    vector<unsigned> adjacentIDs_of1;
    
    //Search Hash_Intersection_Info if a key with this intersection name exists
    unordered_map<string,Intersection>::iterator iter = Hash_Intersection_Info.find( intersection_name1 );
    
    //Intersection name one doesn't exist
    if (iter == Hash_Intersection_Info.end() )   ;
    
    //We found the Intersection Info of this name
    else
        
        //The adjacent intersections of intersection 1
         adjacentIDs_of1 = (iter->second).Adjacent_IntersectionIDs;
    
    
    //This is now an O(1) order operation, and not an O(n)
    unsigned intersection2_ID = find_intersection_id_from_name(intersection_name2);
    
    //At most, there are usually 4 adjacent intersection IDs in the vector
    //Therefore compare and see if intersection 2 is one of those ids
    //If yes then it is adjacent and it is directly connected to Intersection one
    vector<unsigned>::iterator iter1 = find( adjacentIDs_of1.begin(), adjacentIDs_of1.end(), intersection2_ID);
    
    //This means that it found the adjacent ID and it is directly connected
    if ( iter1 != adjacentIDs_of1.end()  ) return 1;
    
    //It didn't find it, therefore it is not directly connected
    else  return 0;

}


//find all intersections connected by one street segment from given intersection (hint: check for 1-way streets too)
//This information is precomputed in load_map() using the helper function adjacent_intersections(unsigned intersection_ID)
//The adjacent intersection IDs of all Intersections are stored in a vector in the hash table Hash_Intersection_Info
std::vector<unsigned> find_adjacent_intersections(std::string intersection_name)
{
    //Search to see if there is a key with this intersection name in the hash table
    unordered_map<string,Intersection>::iterator iter = Hash_Intersection_Info.find( intersection_name );
    
    //Doesn't exist
    if (iter == Hash_Intersection_Info.end() )
    {
        return vector<unsigned>{};
    }
    
    //Exists
    else
        
        return (iter->second).Adjacent_IntersectionIDs;

}


//for a given street, return all the street segments
//This information is precomputed in load_map()
//Hash_Street_Info includes a vector of all the street segments for a street name
std::vector<unsigned> find_street_street_segments(std::string street_name)
{
   
    //Search Hash_Street_Info to see if a key with mentioned street name exists
    unordered_map<string,Street>::iterator iter = Hash_Street_Info.find( street_name );
    
    //It doesn't exist
    if ( iter == Hash_Street_Info.end() )
     {
       
        return vector<unsigned>();
     }
    
    //Exists
    else
        return( (iter->second).All_Street_Segment_IDs );
    
}


//for a given street, find all the intersections
//This is computed in load_map()
//Hash_Street_Info contains Street Objects
//Street Objects include a vector of the Intersection IDs of a street
std::vector<unsigned> find_all_street_intersections(std::string street_name)
{
    //Search Hash_Street_Info to see if a key with mentioned street name exists
    unordered_map<string,Street>::iterator iter = Hash_Street_Info.find( street_name  );
    
    //Then it exists
    if ( iter != Hash_Street_Info.end() ) return ( (iter->second).All_intersection_IDs );
    
    else 
        
    {
                return vector<unsigned>{};
    }
 
    
}


//Find distance between two coordinates
//Note: <cmath> computes cos in radians, not degrees
//Therefore convert latitude and longitude of each point to radians
//(x, y) = (lon · cos(latavg), lat)
//Use formula in milestone 1 handout
double find_distance_between_two_points(LatLon point1, LatLon point2)
{
    double distance_between_2_points;
    
    if ( (point1.lat == point2.lat) && (point1.lon == point2.lon) ) return 0;
    
    //To project part of the earth's surface on to a flat plane and use x-y axis
    //Easier than using LatLon points
    //(x,y) coordinates of the two points
    double x1, x2, y1, y2;
    
    double Average_Latitude_Degrees = (point1.lat + point2.lat) / 2;
    
    //Convert it to Radians
    double Average_Latitude_Radians = Average_Latitude_Degrees * DEG_TO_RAD;
    
     //In Radians
     y1 = point1.lat * DEG_TO_RAD;
     y2 = point2.lat * DEG_TO_RAD;
    
     x1 = point1.lon * DEG_TO_RAD * cos(Average_Latitude_Radians);
     x2 = point2.lon * DEG_TO_RAD * cos(Average_Latitude_Radians);
    
    distance_between_2_points = EARTH_RADIUS_IN_METERS * sqrt( ((y2-y1)*(y2-y1)) + ((x2-x1)*(x2-x1)) );
    
    return distance_between_2_points;
}


//find the length of a given street segments
//Use the cached data in Hash_StreetSegment_Info
double find_street_segment_length(unsigned street_segment_id)
{
    unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find(street_segment_id);
    
    if (iter != Hash_StreetSegment_Info.end() )
        
        return ( (iter->second).StreetSegmentLength );
    
}

//find the length of a whole street
//This information is stored in the Street object in Hash_Street_Info
//If the the street name entered is valid
//Then just return the length from the hash table
 double find_street_length(std::string street_name)
{
    //Search the hash table to see if a matching key name exists
    unordered_map<string,Street>::iterator iter = Hash_Street_Info.find( street_name  );
    
    //If it exists
    if ( iter != Hash_Street_Info.end() ) return ((iter->second).length);
   
    //Doesn't exist
    else 
     {
        return 0;
     }
    
}


//find the travel time to drive a street segment (time(minutes) = distance(Km)/speed_limit(Km/h)*60)
//Maybe check if you can drive on this street at all? (ie: It is a one-way sreet)
double find_segment_travel_time(unsigned street_segment_id)
{
    double time;
    
    //Divide by 1000 to convert it to kilometers
    double street_seg_length = ( find_street_segment_length (street_segment_id) ) /1000;
    double street_seg_speed_limit = getStreetSegmentSpeedLimit( street_segment_id );
    
    time = street_seg_length / street_seg_speed_limit * 60;
    
    return time;
    
    
}


//find the nearest point of interest (by name) to a given position
//There are a total of ~9000 POI so maybe looping over them won't take too much time
std::string find_closest_point_of_interest(LatLon my_position)
{
    //First Set an inital shortest_distance that can be compared with other distances later on
    //And then keep on storing the shortest distance and erasing the previous longer one
    //We initially choose the closest distance to be between our position and the 0th POI
    double old_shortest_distance = find_distance_between_two_points ( my_position, getPointOfInterestPosition(0) );
    unsigned Nearest_POI_ID = 0;
    
    //Starts at 1 because we already stored the distance from the 0th POI to my_position
    for (int i = 1; i < getNumberOfPointsOfInterest() ; i++)
    {
        LatLon POI_Location = getPointOfInterestPosition( i );
        double new_distance =find_distance_between_two_points( my_position,POI_Location );
        
        //If this new POI is closer than the one previously saved then change the Nearest POI ID to this new ID
        if ( new_distance  < old_shortest_distance )
        {
            Nearest_POI_ID = i;
            old_shortest_distance = new_distance;
        }
    }
    
    std::string Nearest_POI_Name = getPointOfInterestName( Nearest_POI_ID );
    
    return Nearest_POI_Name;
    
}

/*******************************Helper Functions******************************************/

//find the length of a whole street
//Find all the street segments of this street in a vector
//Add up individual street segment lengths using find_street_segment_length()
double Calculate_street_length(std::string street_name)
{
    
    long double Street_Length = 0;
    
    std::vector<unsigned> Street_StreetSegments = find_street_street_segments( street_name );
    
    for (int i = 0; i < Street_StreetSegments.size() ; i++)
    {
        Street_Length = Street_Length + find_street_segment_length( Street_StreetSegments[i] );
    }
    
    return Street_Length;
}


//This helper function is used in load_map() when looping through all the intersections
//For each intersection we see what are its street names
//We add this intersection ID to a Hash Table with key = the street names and value = Street Object
//Objective is to have a hash table with key = street names and each street name having a value containing a vector of all the intersection IDs the street goes through
void All_IntersectionIDs_for_Street(unsigned IntersectionID)
{
        std::vector<std::string> Intersection_StreetNames;
    
        Intersection_StreetNames = find_intersection_street_names(IntersectionID);
        
        for (int j = 0; j < Intersection_StreetNames.size() ; j++)
        {
        
                //Search Hash_Street_Info to see if a matching key exists
                unordered_map<string,Street>::iterator iter = Hash_Street_Info.find( Intersection_StreetNames[j] );
            
                //It exists
                if ( iter != Hash_Street_Info.end())
                
                    //Add this Intersection ID to the vector containing all the Intersection IDs of this street
                    ((iter->second).All_intersection_IDs).push_back(IntersectionID);
        
                //Doesn't exist
                else if ( iter == Hash_Street_Info.end())
            
                {
                    vector<unsigned> initial_Add = { IntersectionID };
                    Street temp(initial_Add);
                    Hash_Street_Info.insert( make_pair( Intersection_StreetNames[j] , temp) );
                }
            
        }
    
}

//Purpose of function:
//Returns vector of adjacent intersections to be stored in Hash_Intersection_Info
//BUT it also takes in a vector by reference
//It has to fill up this vector by the adjacent Intersection IDs AND their respective street segment IDs
//This second vector will be stored in Hash_IntersectionNode_Info


//First get the Intersection ID of this intersection
//Then get a vector of the IDs of the street segments attached to this intersection
//Check each street segment to see if it is one way (thus use getStreetSegmentEnds to find the adjacent intersection id) and if it is a 2 way segment then use getStreetSegmentEnds as well to find the adjacent intersection (Note: for a 2 way segment, it requires a bit more work as the from and to Intersection IDs returned are arbitrary)
vector<unsigned> adjacent_intersections(unsigned intersection_ID, vector<AdjID_and_StSegID>& AdjIDs_and_SegIDs)
{
    std::vector<unsigned> adjacent_Intersection_IDs;
    std::vector<unsigned> Intersection_StreetSegment_IDs = find_intersection_street_segments(intersection_ID);
    // unsigned intersection_ID = find_intersection_id_from_name( intersection_name );
    
    for (int i = 0; i < Intersection_StreetSegment_IDs.size() ; i++)
    {
        //if the street segment is a one way street
        if ( getStreetSegmentOneWay( Intersection_StreetSegment_IDs[i] ) )
        {
            //Check the "from" and "to" Intersection IDs of the street segment
            StreetSegmentEnds StreetSeg_FromTo_IntersectionID = getStreetSegmentEnds( Intersection_StreetSegment_IDs[i] );
            
            if ( StreetSeg_FromTo_IntersectionID.from == intersection_ID) //Then valid adjacent intersection
                
            {
                //Add the "To" portion of the struct StreetSegmentEnds (the final intersection ID) to our vector we want to return
                adjacent_Intersection_IDs.push_back( StreetSeg_FromTo_IntersectionID.to );
            
                
                //Store the "To" intersection ID and the street segment ID joining them in a struct AdjID_and_StSegID
                AdjID_and_StSegID temp( StreetSeg_FromTo_IntersectionID.to , Intersection_StreetSegment_IDs[i] );
                
                //Add this temp struct to the vector which contains All the adjacent IDs and all the Street segment IDs of that intersection
                AdjIDs_and_SegIDs.push_back(temp);
            }
            
        }
        
        else //It is a two way street segment
        {
            //Check the "from" and "to" Intersection IDs of the street segment
            StreetSegmentEnds StreetSeg_FromTo_IntersectionID = getStreetSegmentEnds( Intersection_StreetSegment_IDs[i] );
            
            //Now since the street segment is two-way, the "from" and "to" IDs are arbitrary
            //Check which ID matches our current/intial intersection
            //Therefore the other one means that it is the adjacent intersection/final intersection
            
            if ( StreetSeg_FromTo_IntersectionID.from == intersection_ID )
            {
                adjacent_Intersection_IDs.push_back( StreetSeg_FromTo_IntersectionID.to );
                
                //Store the "To" intersection ID and the street segment ID joining them in a struct AdjID_and_StSegID
                AdjID_and_StSegID temp( StreetSeg_FromTo_IntersectionID.to , Intersection_StreetSegment_IDs[i] );
                
                //Add this struct to the vector containing all of them
                AdjIDs_and_SegIDs.push_back(temp);
            }
            
            else if ( StreetSeg_FromTo_IntersectionID.to == intersection_ID )
                
            {
                adjacent_Intersection_IDs.push_back( StreetSeg_FromTo_IntersectionID.from );
                
                
                AdjID_and_StSegID temp( StreetSeg_FromTo_IntersectionID.from , Intersection_StreetSegment_IDs[i] );
                
                //Add this struct to the vector containing all of them
                AdjIDs_and_SegIDs.push_back(temp);

            }
            
        }
    }
    
    return adjacent_Intersection_IDs;
}



//Converts a LatLon point to the XY coordinate system and returns a t_point struct
t_point Convert_LatLon_to_XY(LatLon Point)
{
    t_point coordinate;
    
    //Convert it to Radians
    double MapAverageLatitude_Radians = MapAverageLatitude * DEG_TO_RAD;
    
    coordinate.x = Point.lon * DEG_TO_RAD * cos( MapAverageLatitude_Radians ) ;
    coordinate.y = Point.lat * DEG_TO_RAD ;
    
    
    return coordinate;

}


//Converts an XY coordinate and to a LatLon point
//Does exactly the reverse of the above function
LatLon Convert_XY_to_LatLon(float x, float y)
{
    LatLon Point;
   
    //Convert it to Radians
    double MapAverageLatitude_Radians = MapAverageLatitude * DEG_TO_RAD;
    
    Point.lon = x / ( DEG_TO_RAD * cos( MapAverageLatitude_Radians )  );
    Point.lat = y / DEG_TO_RAD;
    
    return Point;
}



//Loads a vector<t_point> of all the (x,y) coordinates of the points of the street segment (including the .from and .to points and all the curve points)
//This function also computes the street segment length at the same time for efficiency
//Both parameters are passed in by reference
//Use getStreetSegmentEnds() to get the "from" and "to" Intersection IDs of this street segment
//Then use getIntersectionPosition() to get the LatLon coordinates of the "from" & "to" Intersection IDs
//Finally use find_distance_between_two_points() to compute the distance (in metres)/ or equivalently the length of the street segment
void Coordinates_of_StreetSegment_Points_AND_ComputeStSegLength(vector<t_point>& VectorXYCoordinates , int StreetSegmentID, double& StreetSegment_Length)
{
    
    //Use getStreetSegmentEnds() to get the "from" and "to" Intersection IDs of this street segment
    StreetSegmentEnds from_to_Intersection_ID = getStreetSegmentEnds( StreetSegmentID );
    
    //The LatLon of the start and end of the street segment
    LatLon Position_1 = getIntersectionPosition( from_to_Intersection_ID.from );
    LatLon Position_2 = getIntersectionPosition( from_to_Intersection_ID.to );
    
    //Note: A street segment is not always a straight Line
    //It might be a curve or a Cul-de-sac (ie: it might have curve points)
    unsigned Num_StreetSeg_CurvePoints = getStreetSegmentCurvePointCount( StreetSegmentID );
    
    //Now we have two situations
    //Either it has 0 curve points (straight segment)
    //Or it has a minimum of 1 curve points
    
 
    
    if ( Num_StreetSeg_CurvePoints == 0 ) //Therefore straight segment
    {
        // load the XY vector
        // Make sure the order of inserting is correct
         VectorXYCoordinates.push_back( Convert_LatLon_to_XY(Position_1) );
         VectorXYCoordinates.push_back( Convert_LatLon_to_XY(Position_2) );
        
        StreetSegment_Length = find_distance_between_two_points( Position_1, Position_2 );
       

    }
    
    else //It has curve points
    
    {
         double Sum_of_StreetSegs_WithCurve = 0;
        
        //Store the (x,y) coordinate of the first point of the street segment
        VectorXYCoordinates.push_back( Convert_LatLon_to_XY(Position_1) );
        
        //Store the (x,y) coordinate of the curve points of the street segment
        for (int i = 0; i < Num_StreetSeg_CurvePoints ; i++)
        {
            
            LatLon CurvePointLatLon = getStreetSegmentCurvePoint(StreetSegmentID, i);
            
            //We have to store a t_point struct in the vector
            //So use Convert_LatLon_to_XY() to get a t_point of the (x,y) coordinates
            VectorXYCoordinates.push_back( Convert_LatLon_to_XY(CurvePointLatLon)  );
            
            if ( i < (Num_StreetSeg_CurvePoints-1) )
            {
            //idx starts from 0 until Num_StreetSeg_CurvePoints-1
            //First compute the distance between the first and the last curve point of the street segment
                Sum_of_StreetSegs_WithCurve = Sum_of_StreetSegs_WithCurve + find_distance_between_two_points( getStreetSegmentCurvePoint( StreetSegmentID, i) , getStreetSegmentCurvePoint( StreetSegmentID, i+1) );
            }
            
            
        }
        
        
        //Store the (x,y) coordinate of the last point of the street segment
        VectorXYCoordinates.push_back( Convert_LatLon_to_XY(Position_2) );
        
        
        //Before we found the distance from the 1st to the last curve point
        //Now add this to the distance from the beginning of the street segment to the 1st curve point and to the distance from the last curve point to the end of the street segment
        //In order the get the complete street length
        StreetSegment_Length = Sum_of_StreetSegs_WithCurve + find_distance_between_two_points(Position_1, getStreetSegmentCurvePoint( StreetSegmentID, 0) ) + find_distance_between_two_points(Position_2, getStreetSegmentCurvePoint( StreetSegmentID, Num_StreetSeg_CurvePoints-1)   );
       
    }
    
}


//Is the Feature ID inputted closed?
//Check to see if the feature has the same first and last LatLon points
//If yes then it is a closed feature
//1 means closed and 0 means open
bool Is_It_Closed (int Feature_ID,  int Num_Points_of_Feature)
{
    
    //The number of points are from 0 to ( getFeaturePointCount() - 1 )
    //if the feature has the same first and last LatLon points
    //Then it is closed
    
    //Check the first and last points
    LatLon Initial_Pt, Last_Pt;
    
    Initial_Pt = getFeaturePoint( Feature_ID, 0 );
    Last_Pt = getFeaturePoint( Feature_ID, (Num_Points_of_Feature-1) );
    
    if ( (Initial_Pt.lat == Last_Pt.lat) && (Initial_Pt.lon == Last_Pt.lon)  ) return 1;
    
    //else it is an open feature
    else return 0;
        
}


//Get all the (x,y) coordinates of the feature in a vector
//Get the LatLon of all the points of that feature using getFeaturePoint()
//Convert LatLon to XY coordinates using m1.h function
//Add the struct t_point (containing the X and Y coordinates) to the vector
vector<t_point> getXY_of_Feature (int Feature_ID, int Num_Points_of_Feature)
{
    vector<t_point> XY_coords;
    
    for (int j = 0; j < Num_Points_of_Feature; j++)
    {
        //Get the LatLon of one the points on that feature
        LatLon temp = getFeaturePoint( Feature_ID, j);
        
        //Convert the LatLon to t_point(X,Y)
        //Push the t_point to the end of the vector
        XY_coords.push_back(  Convert_LatLon_to_XY(temp)  );
        
    }
    
    
    return XY_coords;
}



void Clear_Global_Variables()
{
    
//    for (std::unordered_map<string,Street>::iterator iter = Hash_Street_Info.begin();
//         iter != Hash_Street_Info.end();
//         iter++)
//    {
//        Hash_Street_Info.erase(iter->first);
//        iter = Hash_Street_Info.begin();
//    }
//    
//    for (std::unordered_map<string,Intersection>::iterator iter = Hash_Intersection_Info.begin();
//         iter != Hash_Intersection_Info.end();
//         iter++)
//    {
//        Hash_Intersection_Info.erase(iter->first);
//        iter = Hash_Intersection_Info.begin();
//    }
// 
//    
//    for (std::unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.begin();
//         iter != Hash_StreetSegment_Info.end();
//         iter++)
//    {
//        Hash_StreetSegment_Info.erase(iter->first);
//        iter = Hash_StreetSegment_Info.begin();
//    }
//
//    
//    for (std::unordered_map<int,Feature>::iterator iter = Hash_Feature_Info.begin();
//         iter != Hash_Feature_Info.end();
//         iter++)
//    {
//        Hash_Feature_Info.erase(iter->first);
//        iter = Hash_Feature_Info.begin();
//    }
//    
//    
//    for (std::unordered_map<int,IntersectionNode>::iterator iter = Hash_IntersectionNode_Info.begin();
//         iter != Hash_IntersectionNode_Info.end();
//         iter++)
//    {
//        Hash_IntersectionNode_Info.erase(iter->first);
//        iter = Hash_IntersectionNode_Info.begin();
//    }
//    
//    
//    for (std::unordered_map< string,vector<unsigned> >::iterator iter = Hash_POI_IDs.begin();
//         iter != Hash_POI_IDs.end();
//         iter++)
//    {
//        Hash_POI_IDs.erase(iter->first);
//        iter = Hash_POI_IDs.begin();
//    }
//
//    
//    for (std::unordered_map< unsigned, ClosestIntersection_2POI>::iterator iter = Hash_ClosestIntersection_2POI_Info.begin();
//         iter != Hash_ClosestIntersection_2POI_Info.end();
//         iter++)
//    {
//        Hash_ClosestIntersection_2POI_Info.erase(iter->first);
//        iter = Hash_ClosestIntersection_2POI_Info.begin();
//    }
//    
//        
//    {
//        vector<unsigned>::iterator iter = StreetSegIDs_Highway.begin();
//        
//        while (iter != StreetSegIDs_Highway.end())
//        {
//            iter = StreetSegIDs_Highway.erase(iter);
//        }
//    }
//    
//    {
//        vector<unsigned>::iterator iter = StreetSegIDs_Zoom3.begin();
//        
//        while (iter != StreetSegIDs_Zoom3.end())
//        {
//            iter = StreetSegIDs_Zoom3.erase(iter);
//        }
//    }
//    
//    {
//        vector<unsigned>::iterator iter = StreetSegIDs_Zoom2_Grey.begin();
//        
//        while (iter != StreetSegIDs_Zoom2_Grey.end())
//        {
//            iter = StreetSegIDs_Zoom2_Grey.erase(iter);
//        }
//    }
//    
//    
//    
//    {
//        vector<char>::iterator iter = User_Input.begin();
//        
//        while (iter != User_Input.end())
//        {
//            iter = User_Input.erase(iter);
//        }
//        
//    }
//    
//    
//    {
//        vector<string>::iterator iter = Intersections_Saved.begin();
//        
//        while (iter != Intersections_Saved.end())
//        {
//            iter = Intersections_Saved.erase(iter);
//        }
//        
//    }
//    
//    
//    Buffer_UserInput = NULL;
//    
//    {
//        vector<LatLon>::iterator iter = Click_Coordinates.begin();
//        
//        while (iter != Click_Coordinates.end())
//        {
//            iter = Click_Coordinates.erase(iter);
//        }
//        
//    }
//    
//    IntersectionEntered_Found = 0;
//    AllPOIsOn = 0;
//    AllFeaturesOn = 0;
//    PointClicked = 0;
//    POI_Found = 0;
//
//    
//    {
//        vector<unsigned>::iterator iter = The_Path_For_Drawing.begin();
//        
//        while (iter != The_Path_For_Drawing.end())
//        {
//            iter = The_Path_For_Drawing.erase(iter);
//        }
//        
//    }
//    
//    
//    {
//       vector<IntersectionNode*>::iterator iter = Intersections_Examined_DuringSearch.begin();
//        
//        while (iter != Intersections_Examined_DuringSearch.end())
//        {
//            iter = Intersections_Examined_DuringSearch.erase(iter);
//        }
//        
//    }
//    
//    //Clear the vector containing all the intersection names for Auto_Completion
//    {
//        vector<const char*>::iterator iter = Intersection_Names_AutoCompletion.begin();
//        
//        while (iter != Intersection_Names_AutoCompletion.end())
//        {
//            iter = Intersection_Names_AutoCompletion.erase(iter);
//        }
//        
//    }
//    
//    
    
}







