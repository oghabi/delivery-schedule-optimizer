#pragma once //protects against multiple inclusions of this header file
#include "m3.h"



/**********************************Gobal Variables***********************************/

//Use the vector of the path obtained to draw it out in m2.cpp in the draw_screen() function
vector<unsigned> The_Path_For_Drawing;

//Pointers to Intersection IDs we examined while we were searching for a path between 2 nodes
//We use this to clear the visited flags of the Nodes we examined in order for them to be searched
//in future path findings
//This is more efficient than looping through all the Intersection IDs and setting visited = 0
//NOTE: This vector is different from the path, it contains pointers to all nodes we examined but
//didn't include in the final path
vector<IntersectionNode*> Intersections_Examined_DuringSearch;


//The UnitTests.cpp files for all the 3 functions are located in the directory below
//   /cad2/ece297s/public


using namespace std;






// Returns a path (route) between the start intersection and the end
// intersection, if one exists. If no path exists, this routine returns
// an empty (size == 0) vector. If more than one path exists, the path
// with the shortest travel time is returned. The path is returned as a vector
// of street segment ids; traversing these street segments, in the given order,
// would take one from the start to the end intersection.
std::vector<unsigned> find_path_between_intersections(unsigned intersect_id_start, unsigned intersect_id_end)
{
    
    //Clear the visited flags before searching for a path
    //Go through the global vector Intersections_Examined_DuringSearch
    //This vector contains pointers to IntersectionNodes we visited during the path search
    for (int i = 0; i < Intersections_Examined_DuringSearch.size(); i++)
    {
        //Set the visited flag down
        Intersections_Examined_DuringSearch[i]->visited = 0;
    }
   
    
    if ( intersect_id_start == intersect_id_end )
    {
        //If the starting and ending intersections are the same,
        //return empty vector
         return vector<unsigned>();
    }
    
    
    //Find an actual path
    else
    
    {
        unordered_map<int,IntersectionNode>::iterator iter = Hash_IntersectionNode_Info.find( intersect_id_start );
        
        //Initialize this as 0 first. ie. Not found
        bool found = 0;
        
        //Start searching for a path only if the starting intersection ID is valid
        if ( iter !=  Hash_IntersectionNode_Info.end() )
        {
            
            //Pointer to an IntersectionNode in the hash table
            //iter->second returns an IntersectionNode object, so the pointer should be &(iter->second)
            //This will be our starting node
            IntersectionNode* IntersectionNode_ptr = &(iter->second);
            
            found = BFSpath( IntersectionNode_ptr , intersect_id_end );
            
            
        if ( found )
                {
                    //Trace the Path back using the function BFS_Path_Traceback();
                    //Store it in a deque
                    deque<unsigned> path = BFS_Path_Traceback ( intersect_id_end );
                    
                    //We have to return back a vector of the path, not a deque
                    vector<unsigned> The_Path ( path.begin(), path.end() );
                    
                    //Later on no need for this
                    //Just call find_path_between_intersections inside m2.cpp!
                    //The_Path_For_Drawing = The_Path;
                    
                    return The_Path;

               }
            
            //If we didn't find a path or the starting intersection ID was invalid
            //return an empty vector
        else
            return vector<unsigned>();
            
        }
        
        //If we didn't find the initial intersection ID in Hash_IntersectionNode_Info
        return vector<unsigned>();
    }
    
}





bool BFSpath( IntersectionNode* SourceNode, int destID )
{
    
    bool found = 0;
    
    //This will be a minimum heap containing all the WaveElements we have to inspect
    //A WaveElement contains
    //1)A pointer to the IntersectionNode to be inspected
    //2)The StreetSegment ID used to reach this node
    //The WaveFront is now stored as a minHeap
    //The minimum WaveElement is stored at the top
    priority_queue< WaveElement, std::vector<WaveElement>, comparator_WaveElement> WaveFront;
    
    //Add this WaveElement to the list to be inspected first
    //NO_EDGE means that it is our starting node and we reached it with no other previous street segments
    //O means it takes 0 seconds from source to intial node
    //The second 0 doesn't matter, it is the time it takes (idealistically/Heuristics) to go from this node to the destination node
    //Its obviously not zero minutes, but it doesn't matter since its the first element we are inspecting
    WaveFront.push(  WaveElement(SourceNode,NO_EDGE, 0, 0 )  );
    
    //Set Travel Time to inital node as zero
    SourceNode->Travel_Time_from_Source = 0;
    
    double time_from_source ;
    
    //A vector of pointers to the nodes we examined during the path search
    vector<IntersectionNode*> Nodes_Examined;
    
    bool Travel_Time_Dropped;
    
    //Best Path Time until the destination
    double Best_Path_Time;

    
    //Keep on inspecting the WaveElements in the WaveFront list until its empty
    while ( WaveFront.size() != 0 )
    {
        //cout << WaveFront.size() << endl;
        
        //Dijkstra's algorithm: Inspect the top element in the heap first [The minimum WaveElement]
        WaveElement Current_Element_Inspected = WaveFront.top();
        
        //Remove it from the list
        WaveFront.pop();
        
        //This means that we found our first path
        //After this point start checking the WaveFront so that all entries have higher travel time than best path found
        if ( found )
        {
            {
               
               //if ( Current_Element_Inspected.Travel_Time < Best_Path_Time )  //This is Dijkstra's
                if ( Current_Element_Inspected.Heuristic_And_TimeFromInitial < Best_Path_Time )     //This is A*

                   //We found an entry better than our best path length found
                   goto Continue;
            }
            
                //Copy the nodes visited in global variable
                Intersections_Examined_DuringSearch = Nodes_Examined;
            
                //End the path search, we found the best path
                return found;
        }
        
    Continue:
        
      
        Travel_Time_Dropped = ( Current_Element_Inspected.Travel_Time <  (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source );
        
        if ( (Current_Element_Inspected.Node_ptr)->visited == 0 || Travel_Time_Dropped )
        {
            //Update the StSeg ID used to reach this node in the IntersectionNode object in Hash_IntersectionNode_Info
            (Current_Element_Inspected.Node_ptr)->SegID_Reaching_This_Node = Current_Element_Inspected.SegID_reaching_this_node;
            
            //It hasn't been visited before so set the travel time for this node
            (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source = Current_Element_Inspected.Travel_Time;
            
            //If we reached the destination ID, return true (there is a path
            if (  (Current_Element_Inspected.Node_ptr)->IntersectionID == destID  )
            {
                //Dont return 1,we might have better paths in the WaveFront
                //Keep on searching
                found = 1;
                
                //This is the Best Path Time until now
                Best_Path_Time = (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source;
            }

            
        }
    
        
        //Process the IntersectionNode if it hasn't been visited before or if Travel Time Dropped
        if ( (Current_Element_Inspected.Node_ptr)->visited == 0 || Travel_Time_Dropped )
        {
        //The number of adjacent Intersections
        //From the WaveElement being inspected, access the IntersectionNode
        //From the IntersectionNode object, access the vector inside it which contains all the adjacent Intersec IDs
        //Get the size of this vector
        int Num_AdjacentIDs = ((Current_Element_Inspected.Node_ptr)->AdjIDs_and_SegIDs).size();
        
        //Now store the future elements to be inspected in the back of the WaveFront list
        //These elements are the adjacent nodes of the current node being inspected
        for (int i = 0; i < Num_AdjacentIDs ; i++)
        {
            int AdjID = ( (Current_Element_Inspected.Node_ptr)->AdjIDs_and_SegIDs[i] ).AdjID;
            
            int StSegID_to_ReachNode = ( (Current_Element_Inspected.Node_ptr)->AdjIDs_and_SegIDs[i] ).StSegID;
            
            //This is our heuristic function h(t)
            double Time_to_Destination;
            
            //add the time it takes from the source node until this node
            {
                //Search all the information for this street segment in Hash_StreetSegment_Info
                unordered_map<int,StreetSegment>::iterator iter2 = Hash_StreetSegment_Info.find(StSegID_to_ReachNode );
                
                //Time to traverse the connected street segment
                time_from_source = (iter2->second).StreetSegmentLength / ( (iter2->second).StreetSegment_SpeedLimit * KMPH_TO_METREperMIN );
                
                //To get the total time from source, add the travel time stored in the previous node
                time_from_source = time_from_source + (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source;
                
                
                //This is our heuristic for A*
                //We pick the lowest bound
                //No overestimation cuz the slightest overestimation will not give us the shortest path!
                //Our heuristic is that to get from the current node to the final node, the fastest way would be if they were connected with a
                //straight line and you could take the highway driving at 100kmph
                //RIGHT NOW ITS 100 change it to 85 or 90 for milestone 4 for speed!
                Time_to_Destination = find_distance_between_two_points( getIntersectionPosition(AdjID), getIntersectionPosition(destID) ) / (100.0 * KMPH_TO_METREperMIN);
                
                //Add any turn penalties
                //if the segments belong to different street names, then +15s or 0.25min
                
                //This checks that the previous node isn't the initial node
                //Because if it is the initial node then, don't add 0.25min
                if ( Current_Element_Inspected.SegID_reaching_this_node != NO_EDGE)
                {
                    
                    //COMPARING STRINGS ARE SLOW! ints are faster
                    
                    int StID_1 = (iter2->second).StreetID_of_Segment;
                    // string StName2 = (iter2->second).Name_of_StreetSeg;
                    
                    iter2 = Hash_StreetSegment_Info.find(Current_Element_Inspected.SegID_reaching_this_node);
                    
                    int StID_2 = (iter2->second).StreetID_of_Segment;
                    //string StName1 = (iter2->second).Name_of_StreetSeg;
                    
                    //0.25min penalty for different street names
                    if (StID_1 != StID_2)   time_from_source = time_from_source + 0.25;
                    
                }
            }
        
            
            //Look for the adjacent ID in Hash_IntersectionNode_Info to get its IntersectionNode object
            unordered_map<int,IntersectionNode>::iterator iter1 = Hash_IntersectionNode_Info.find( AdjID );
            
            WaveFront.push( WaveElement( &(iter1->second),StSegID_to_ReachNode, time_from_source, Time_to_Destination ) );
        }
        
            if ( (Current_Element_Inspected.Node_ptr)->visited == 0)
            {
            //Mark this IntersectionNode as visited
            (Current_Element_Inspected.Node_ptr)->visited = 1;
            
            //Add this node to the list of nodes examined
            Nodes_Examined.push_back( Current_Element_Inspected.Node_ptr );
            }

            
        } //End if node has not been visited yet
        
        
        Travel_Time_Dropped = 0;
 
        
    }//End while WaveFront is not empty

    //Copy the nodes visited in global variable
    Intersections_Examined_DuringSearch = Nodes_Examined;
    
    return found;
}





//Function to return the path found in a deque/vector
//It returns all the StreetSegment IDs that we have to travel through
//We find the path by going backwards from the destination Node to the starting Node
//In each IntersectionNode, we have a memeber telling us what street segment ID was used to reach it
deque<unsigned> BFS_Path_Traceback (int destID)
{
    
    deque<unsigned> path;
    
    //Find the IntersectionNode object which belongs to the destinationID
    unordered_map<int,IntersectionNode>::iterator iter = Hash_IntersectionNode_Info.find( destID );
    
    //Make a pointer to that object found
    IntersectionNode* IntersectionNode_ptr = &(iter->second);
    
    //Find the StSeg ID used to reach the final destination node
    int StSegID_Used_to_ReachNode = IntersectionNode_ptr->SegID_Reaching_This_Node;
    
    //Keep going backwards to find the path of street segments
    //until we reach the source node
    while( StSegID_Used_to_ReachNode != NO_EDGE )
    {
        path.push_front( StSegID_Used_to_ReachNode );
        
        
        // cout << "Visited: " << IntersectionNode_ptr->visited << " InterID: " << IntersectionNode_ptr->IntersectionID << "|" << "SegmentReachingThisNode: " <<StSegID_Used_to_ReachNode <<"  ONE WAY: " <<getStreetSegmentOneWay(StSegID_Used_to_ReachNode) <<endl;
        
        
        //Find the "from" and "to" IDs of this street segment
        //In order to trace the nodes backwards to the source node
        int PrevNodeID = Get_Previous_Node( StSegID_Used_to_ReachNode, IntersectionNode_ptr->IntersectionID  );
        
        //Find the IntersectionNode object belonging to the previous node that got us here
        iter = Hash_IntersectionNode_Info.find( PrevNodeID );
        
        //Make a pointer to that object found
        IntersectionNode_ptr = &(iter->second);
        
        StSegID_Used_to_ReachNode = IntersectionNode_ptr->SegID_Reaching_This_Node;
    }
    
    return path;
}


//Get the previous node
//As we are tracing "backwards" to find the path
int Get_Previous_Node ( int SegID, int CurrentNodeID )
{
    //Check the "from" and "to" Intersection IDs of the street segment
    StreetSegmentEnds StreetSeg_FromTo_IntersectionID = getStreetSegmentEnds( SegID );
    
    //if the .to portion is our CurrentNodeID
    //Then the .from portion is the previous node that got us here
    if ( StreetSeg_FromTo_IntersectionID.to == CurrentNodeID)
    {
        return ( StreetSeg_FromTo_IntersectionID.from );
    }
    
    //Then the .from portion is our CurrentNodeID
    //Then the .to portion is the previous node that got us here
    else
        return ( StreetSeg_FromTo_IntersectionID.to );
}

/*************************************************Should be in comparator_ClosestIntersection_2POI.h*******************************/
//This is our custom made less than operator function
//It is not named as "operator<" because our inputs are TWO pointers (ClosestIntersection_2POI*)
//You can only name it "operator<" if the LHS and the RHS of "<" are objects/structs
//Note: We use this custom made function to store ClosestIntersection_2POI* in a Min Heap
bool LessThanOP_ClosestIntersection_2POI ( ClosestIntersection_2POI* lhs, ClosestIntersection_2POI* rhs  )
{
    if ( lhs->Dist_POI_2_StartID < rhs->Dist_POI_2_StartID ) return 0;
    
    else return 1;
}

//Used to store the pointers (ClosestIntersection_2POI*) in a Min Heap
struct comparator_ClosestIntersection_2POI
{
    bool operator() ( ClosestIntersection_2POI* i, ClosestIntersection_2POI* j)
    {
        bool answer = LessThanOP_ClosestIntersection_2POI(i, j);
        return answer;
    }
};
/****************************************************************************************************************************/


// Returns the shortest travel time path (vector of street segments) from
// the start intersection to a point of interest with the specified name.
// If no such path exists, returns an empty (size == 0) vector.
std::vector<unsigned> find_path_to_point_of_interest (unsigned intersect_id_start, std::string point_of_interest_name)
{
//    
//    double Fastest_Time_to_POI;
//    vector<unsigned> Fastest_Path;
//    
//    bool First_Path_Found = 1;
//    
//    int count = 0;
//    
//    //MinHeap is a vector of pointers
//    //Create a MinHeap to store pointers to objects of ClosestIntersection_2POI
//    //Struct ClosestIntersection_2POI contains all the information about the closest Intersection ID of a POI
//    //This MinHeap is organized based on: The distance from the Starting Intersection to the Intersection ID closest to the POI searched
//    priority_queue<ClosestIntersection_2POI*, std::vector<ClosestIntersection_2POI*>, comparator_ClosestIntersection_2POI> MinHeap_ClosestIntersection_2POI_Ptr;
//    
//    //Look for the IDs of the POI entered in Hash_POI_IDs
//    std::unordered_map< string,vector<unsigned> >::iterator iter = Hash_POI_IDs.find( point_of_interest_name );
//    
//    //(iter->second) is a vector<unsigned> which contains all the IDs of the POI name entered
//    
//    //We found the POI name entered
//    if ( iter != Hash_POI_IDs.end() )
//    {
//        
//        LatLon InitialID_Position = getIntersectionPosition( intersect_id_start );
//        
//        std::unordered_map< unsigned, ClosestIntersection_2POI>::iterator iter1;
//        
//        for (int i = 0; i < (iter->second).size(); i++)
//        {
//            iter1 = Hash_ClosestIntersection_2POI_Info.find( iter->second[i] );
//            
//            //There may be many POI IDs associated to the searched name
//            //Calculate Distances from the POI IDs to our starting initial Intersection ID
//            //And store them in Hash_ClosestIntersection_2POI_Info
//            (iter1->second).Dist_POI_2_StartID = find_distance_between_two_points( InitialID_Position, (iter1->second).LatLon_of_POI );
//            
//            
//            //Now we have the distance from the searched POI IDs to our current location
//            //Store them in a min heap based on their distance from the current location
//            MinHeap_ClosestIntersection_2POI_Ptr.push( &(iter1->second) );
//        }
//        
//    
//            //Now we have have limited the information to be searched and organized it in the min heap
//            //In the min heap, we begin with searching for POIs which are close to the starting intersection
//    
//        
//       
//            //Keep on inspecting the heap until it becomes empty
//            //Only compare the path between the first 2 closest POIs which are already organized in the min heap
//            while ( MinHeap_ClosestIntersection_2POI_Ptr.size() != 0 && count < 2)
//            {
//                ClosestIntersection_2POI* Ptr_2_ClosestIntersection_ofPOI = MinHeap_ClosestIntersection_2POI_Ptr.top();
//                
//                MinHeap_ClosestIntersection_2POI_Ptr.pop();
//                count++;
//                
//                int ClosestIntersectionID = Ptr_2_ClosestIntersection_ofPOI->Closest_IntersectionID;
//                
//                //If the closest Intersection ID is our starting Intersection ID, then return empty vector
//                if ( ClosestIntersectionID == intersect_id_start )
//                    return vector<unsigned>();
//                
//                vector<unsigned> Temp_Path = find_path_between_intersections( intersect_id_start, ClosestIntersectionID );
//                
//                //Only compute the path travel time if the vector is not empty
//                //ie. Compute travel time if there EXISTS a path between the starting Intersection and the Intersection ID closest to the POI
//                if ( Temp_Path.size() != 0 )
//                {
//                    
//                    if ( First_Path_Found )
//                    {
//                        Fastest_Path = Temp_Path;
//                        Fastest_Time_to_POI = compute_path_travel_time( Fastest_Path );
//                        First_Path_Found = 0;
//                    }
//                    
//                    else
//                    {
//                        double Temp_PathTime = compute_path_travel_time( Temp_Path );
//                        if ( Temp_PathTime < Fastest_Time_to_POI )
//                        {
//                            Fastest_Path = find_path_between_intersections( intersect_id_start, ClosestIntersectionID );
//                            Fastest_Time_to_POI = compute_path_travel_time( Fastest_Path );
//                        }
//                        
//                    }
//                    
//                }
//            }
//    
//    return Fastest_Path;
//        
//    }// end first if statement
//    
//    //The POI name entered doesn't exist, so return an empty vector
//    else
        return vector<unsigned>();

}






// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
// NOTE: The return time should be in minutes!
double compute_path_travel_time(const std::vector<unsigned>& path)
{
    
    //If the path entered is empty
    //ie: Either path doesn't exist or the start & end points are the same intersection
    //return 0; ie. it needs 0 time to travel that path
    if ( path.size() == 0 )    return 0;
    
    
    //The path is made up of two ore more street segments
    else
        
    {
        //Total trave time for the path
        double Total_Travel_Time = 0;
        
        //Travel time for one piece of segment
        double Travel_Time_for_Segment=0;
        
        //For each turn penalty, it is 15 seconds
        //Like a count variable
        int Turn_Penalty = 0;
        
        //In order to check for a 15s turn penalty
        //Store the name of street the first street segment belongs to and store the name of street the consecutive
        //street segment belongs to. Check if they are the same street name
        // string Name_of_Seg1, Name_of_Seg2;
        
        
        //Go through the path vector
        for( int i = 0 ; i < path.size() ; i++ )
            
        {
            //Search for the information of this street segment id in Hash_StreetSegment_Info
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( path[i] );
            
            //Time to travel a street segment = segment length/speed limit
            //Careful: Speed Limit is in Km/Hr. Convert it to m/min
            Travel_Time_for_Segment = (iter->second).T_from_source;
            
            
            //Add the segment travel time to the overall time it will take
            Total_Travel_Time = Total_Travel_Time + Travel_Time_for_Segment;
            
            //Name_of_Seg1 = (iter->second).Name_of_StreetSeg;
            int StID_1 = (iter->second).StreetID_of_Segment;
            
            //Computing the turn penalties for this path
            //Compute turns if we have 2 or more street segments in the path
            
            //We need the -1 because we are comparing street segment names in pairs of two
            if ( i < (path.size()-1) )
                
            {
                //Search for the name of the consectuive
                iter = Hash_StreetSegment_Info.find( path[i+1] );
                
                //Name_of_Seg2 = (iter->second).Name_of_StreetSeg;
                int StID_2 = (iter->second).StreetID_of_Segment;
                
                //If the consecutive street segments have different street names, there is a 15s turn penalty
                //if ( Name_of_Seg1 != Name_of_Seg2 ) Turn_Penalty++;
                if ( StID_1 != StID_2 ) Turn_Penalty++;
                
            }
            
        }
        
        //Add the turn penalties we had to make
        //0.25min = 15/60
        //We are converting the 15 seconds Turn Penalty to minutes
        Total_Travel_Time = Total_Travel_Time + ( Turn_Penalty * 0.25 );
        
        return Total_Travel_Time;
        
    }
    
    
}








// Returns the time required to travel along the path specified. The path
// is passed in as a vector of street segment ids, and this function can
// assume the vector either forms a legal path or has size == 0.
// The travel time is the sum of the length/speed-limit of each street
// segment, plus 15 seconds per turn implied by the path. A turn occurs
// when two consecutive street segments have different street names.
// NOTE: The return time should be in minutes!
double compute_path_travel_time_ofDeque(const std::deque<unsigned>& path)
{
    
    //If the path entered is empty
    //ie: Either path doesn't exist or the start & end points are the same intersection
    //return 0; ie. it needs 0 time to travel that path
    if ( path.size() == 0 )    return 0;
    
    
    //The path is made up of two ore more street segments
    else
        
    {
        //Total trave time for the path
        double Total_Travel_Time = 0;
        
        //Travel time for one piece of segment
        double Travel_Time_for_Segment=0;
        
        //For each turn penalty, it is 15 seconds
        //Like a count variable
        int Turn_Penalty = 0;
        
        //In order to check for a 15s turn penalty
        //Store the name of street the first street segment belongs to and store the name of street the consecutive
        //street segment belongs to. Check if they are the same street name
       // string Name_of_Seg1, Name_of_Seg2;
        
        
        //Go through the path vector
        for( int i = 0 ; i < path.size() ; i++ )
            
        {
            //Search for the information of this street segment id in Hash_StreetSegment_Info
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( path[i] );
            
            //Time to travel a street segment = segment length/speed limit
            //Careful: Speed Limit is in Km/Hr. Convert it to m/min
            Travel_Time_for_Segment = (iter->second).T_from_source;
            
            
            //Add the segment travel time to the overall time it will take
            Total_Travel_Time = Total_Travel_Time + Travel_Time_for_Segment;
            
            //Name_of_Seg1 = (iter->second).Name_of_StreetSeg;
            int StID_1 = (iter->second).StreetID_of_Segment;
            
            //Computing the turn penalties for this path
            //Compute turns if we have 2 or more street segments in the path
            
            //We need the -1 because we are comparing street segment names in pairs of two
            if ( i < (path.size()-1) )
                
            {
                //Search for the name of the consectuive
                iter = Hash_StreetSegment_Info.find( path[i+1] );
                
                //Name_of_Seg2 = (iter->second).Name_of_StreetSeg;
                int StID_2 = (iter->second).StreetID_of_Segment;
                
                //If the consecutive street segments have different street names, there is a 15s turn penalty
                //if ( Name_of_Seg1 != Name_of_Seg2 ) Turn_Penalty++;
                if ( StID_1 != StID_2 ) Turn_Penalty++;
                
            }
            
        }
        
        //Add the turn penalties we had to make
        //0.25min = 15/60
        //We are converting the 15 seconds Turn Penalty to minutes
        Total_Travel_Time = Total_Travel_Time + ( Turn_Penalty * 0.25 );
        
        return Total_Travel_Time;
        
    }
    
    
}




























