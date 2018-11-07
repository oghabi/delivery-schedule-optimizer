 #pragma once //protects against multiple inclusions of this header file
#include "m4.h"



using namespace std;

//Global Data Structure for Milestone 4
//Precomputing all the paths with Dijkstra
//HashTable with Key = the delivery IDs and Value = Another HashTable with key = Delivery IDs and value = Path_and_Time structure
std::unordered_map<int, std::unordered_map<int, Path_and_Time>  > PreComputed_Paths;


//HashTable with key = Starting Delivery ID
std::unordered_map<int, vector<Path_and_Time*>  > Hash_Arranged_Destinations;

//HashTable with key = Delivery IDs and value = Closest Depot ID
std::unordered_map<int, int > Hash_Closest_DepotIDs;




bool WaveElement_LessThan_Dijkstra(WaveElement& lhs , WaveElement& rhs )
{
    if ( lhs.Travel_Time < rhs.Travel_Time ) return 0;
    else return 1;
}

//This comparator organizes the heap only based on Travel_Time from source (so no A*)
//This is used to precompute all the paths in M4
struct comparator_WaveElement_Dijkstra
{
    bool operator() ( WaveElement i, WaveElement j) {
        return ( WaveElement_LessThan_Dijkstra(i,j) ); }
};



//Organizes the IDs in the vector based on the x-coordinate of the ID
bool IntersectionPosition_LessThan(IntersectionNode* lhs , IntersectionNode* rhs )
{
    if ( (lhs->Position).x < (rhs->Position).x ) return 0;
    else return 1;
}


struct comparator_L2R_Arrange
{
    bool operator() ( IntersectionNode* i, IntersectionNode* j)
    {
        return (  IntersectionPosition_LessThan(i,j)  );
    }
    
};


bool SortVector_ptrs (Path_and_Time* i, Path_and_Time* j)
{
    
    if ( i->PathTime < j->PathTime)  return 1;
    
    else return 0;
}



bool SortVectorID (int i, int j)
{
    std::unordered_map<int,IntersectionNode>::iterator iter = Hash_IntersectionNode_Info.find( i );
    
    double x_1 = ((iter->second).Position).x;
    
    iter = Hash_IntersectionNode_Info.find( j );
    
    double x_2 = ((iter->second).Position).x;
    
    
    if ( x_1 < x_2) return 1;
    else return 0;

}

/********************************************************************************************/


//Do Greedy Algorithm for now
std::vector<unsigned> traveling_salesman(std::vector<unsigned> intersections_to_traverse, std::vector<unsigned> depot_locations)
{
    
    //As soon as we call this function
    //Start the timer to make sure we don't go over 30 seconds
    clock_t startTime = clock (); // Clock “ticks”
 
 
    //Only sort it if the number of deliveries
    if ( intersections_to_traverse.size() > 15 )
    {
    sort( intersections_to_traverse.begin() , intersections_to_traverse.end(), SortVectorID);
    }
    
    
    //loop through all the Intersection IDs to traverse
    //We have to call the Dijkstra function 50 times [Hard test cases]
    for (int i = 0; i < intersections_to_traverse.size(); i++)
    {
        //Default closest depot ID to each delivery is depot_locations[0]
        Hash_Closest_DepotIDs.insert( make_pair(intersections_to_traverse[i], depot_locations[0]) );
        
        
        //Fill "Hash_Closest_DepotIDs" to find the closest depot ID to this delivery ID
        //Start from depot_locations[1] because [0] is the default closest depot to all
        for (int m = 1; m < depot_locations.size(); m++)
        {
            LatLon Delivery_Pos;
            LatLon Depot_Old_Pos, Depot_New_Pos;
            
            
            unordered_map<int,IntersectionNode>::iterator iter_LatLon = Hash_IntersectionNode_Info.find( intersections_to_traverse[i] );
            
            Delivery_Pos = (iter_LatLon->second).Pos;
            
            iter_LatLon = Hash_IntersectionNode_Info.find( depot_locations[m] );
            
            Depot_New_Pos = (iter_LatLon->second).Pos;
            
            
            std::unordered_map<int,int>::iterator iter_Closest_Depot = Hash_Closest_DepotIDs.find( intersections_to_traverse[i] );
            
            
            iter_LatLon = Hash_IntersectionNode_Info.find( iter_Closest_Depot->second );
            
            Depot_Old_Pos = (iter_LatLon->second).Pos;

            
            double NewDist = find_distance_between_two_points( Delivery_Pos, Depot_New_Pos );
            
            double OldDist = find_distance_between_two_points( Delivery_Pos, Depot_Old_Pos );
            
            
            //If this depot is closer to the current delivery than the previous depot
            //Set this depot as the closest Depot to this delivery ID
            if ( NewDist <  OldDist )
                
                iter_Closest_Depot->second = depot_locations[m];
        }
        
        
        
        
        //Clear the visited flags before searching for a path
        //Go through the global vector Intersections_Examined_DuringSearch
        //This vector contains pointers to IntersectionNodes we visited during the path search
        for (int k = 0; k < Intersections_Examined_DuringSearch.size(); k++)
        {
            //Set the visited flag down
            Intersections_Examined_DuringSearch[k]->visited = 0;
            
            //Also set the flag "Path_Exists_to_Node" to 0
            //Note: during the Dijkstra search, max 50 nodes can have their Path_Exists_to_Node set to 1
            //These are the delivery nodes
            //So the majority are zero but just set them back to zero in case
            Intersections_Examined_DuringSearch[k]->Path_Exists_to_Node = 0;
        }
        

        
        //Pointer to the starting Intersection in Hash_IntersectionNode_Info
        unordered_map<int,IntersectionNode>::iterator iter = Hash_IntersectionNode_Info.find( intersections_to_traverse[i] );
        
        //Pointer to an IntersectionNode in the hash table
        //iter->second returns an IntersectionNode object, so the pointer should be &(iter->second)
        //This will be our starting node
         IntersectionNode* IntersectionNode_ptr = &(iter->second);
        
        
        
        //Call Dijkstra for M4 to precompute all the paths from one delivery ID to the others
          Dijkstra_M4( IntersectionNode_ptr , intersections_to_traverse );
        
        //Make a temp HashTable which will go inside "PreComputed_Paths"
        std::unordered_map<int,Path_and_Time> temp;
        
        
        //Insert this temp hash table in the hash table "PreComputed_Paths"
        //Insert this empty hash table and fill it up
        PreComputed_Paths.insert( make_pair(intersections_to_traverse[i], temp) );
        
        //Vector containing pointers to Path_and_Time structures
        //Pointers are arranged based on the next closest delivery ID to the current ID
        vector<Path_and_Time*> Vec_Arranged_Destinations;

        //If we are going from node 0 to the other 49 destinations
        //And apply dijsktra but the path to all the other nodes is an empty vector
        //Then this means that this node isnt connected to the other nodes
        //And we should immediately return an empty vector for the courier path
        int Num_Empty_Path = 0;
        
        //Retrieve all the paths from this intial delivery ID to the other delivery IDs
        for (int j = 0; j < intersections_to_traverse.size(); j++)
        {
            
            deque<unsigned> Path_to_DeliveryID;
            double Path_TravelTime;
            
            //We want all the path to the OTHER delivery IDs
            //We don't want the path from the intial delivery ID to itself...
            if ( intersections_to_traverse[j] != intersections_to_traverse[i] )
            {
                iter = Hash_IntersectionNode_Info.find( intersections_to_traverse[j] );
                
                //If a path exists to this node, then trace it back
                if ( (iter->second).Path_Exists_to_Node )
                {
                    //j is the destination ID
                    Path_to_DeliveryID = BFS_Path_Traceback( intersections_to_traverse[j] );
                    Path_TravelTime = compute_path_travel_time_ofDeque( Path_to_DeliveryID );
                }
                
                //Store an empty vector as the path to this delivery node
                //to indicate that no path exists to this delivery node
                else
                {
                    deque<unsigned> Empty_Deque = {};
                    Path_to_DeliveryID = Empty_Deque;
                    Path_TravelTime = 0;
                    
                    //IF this = 49 at the end
                    //it means that this node is isolated and you should return an empty vector
                    Num_Empty_Path++;
                }
             
                //A temporary Path_and_Time struct which will go in the inner hash table
                Path_and_Time temp1(Path_to_DeliveryID,Path_TravelTime,intersections_to_traverse[j]);
                
                
                std::unordered_map<int, std::unordered_map<int, Path_and_Time> >::iterator iter_PreCompPath = PreComputed_Paths.find( intersections_to_traverse[i] );
                
                
                (iter_PreCompPath->second).insert( make_pair( intersections_to_traverse[j], temp1) );
                
                
                std::unordered_map<int,Path_and_Time>::iterator iter_inner_hash = (iter_PreCompPath->second).find(  intersections_to_traverse[j] );
                
                //Insert the delivery node in the vector Vec_Arranged_Destinations
                Path_and_Time* Ptr_2_Path_and_Time = &(iter_inner_hash->second);
                
                
                Vec_Arranged_Destinations.push_back( Ptr_2_Path_and_Time );
            }
            
        }
        
        
        //This node isn't connected to the other nodes
        //So a valid path won't exist in the end
        if ( Num_Empty_Path == (intersections_to_traverse.size() - 1) ) return vector<unsigned>();
        
        //Sort the vector before inserting it
        //Sort the closest Delivery IDs to the starting one
        sort(Vec_Arranged_Destinations.begin(), Vec_Arranged_Destinations.end(), SortVector_ptrs);

        
        //Insert this in the hash table "Hash_Arranged_Destinations"
        //It sorts which one of the other 49 delivery IDs are closest to the current starting one
        Hash_Arranged_Destinations.insert( make_pair(intersections_to_traverse[i], Vec_Arranged_Destinations ) );
        
        
    }//End for loop calling Dijkstra 50 times
    
    

    
    //Now find the order of deliveries
    //The order of the nodes that will be traversed
    vector<unsigned> Order_of_DeliveryIDs;
    
    
    
    double Best_Path_Time = 0;
    
    //Use greedy algorithm for now. Keep on going to the closest delivery location
    
    //Set to monitor which delivery IDs we have visited
     std::unordered_set<int> Set_DeliveryIDs_To_visit( intersections_to_traverse.begin(), intersections_to_traverse.end() );
    
    
    //if the intersections_to_traverse is less than 15 IDs
    //Dont waste time trying to find a basic path with greedy algorithm
    //because we are eventually going to try all the permuations
    //Jump straight into the permuation algorithm
    if ( intersections_to_traverse.size() < 15 )
    {
        Order_of_DeliveryIDs = intersections_to_traverse;
        //This is just an intial best_path_time to compare with future permuations
        Best_Path_Time = FindTime_FromDeliveryList( Order_of_DeliveryIDs );
        goto LessThan15IDs;
    }
    
    
    //This is the closest delivery ID to the left most Depot
    Order_of_DeliveryIDs.push_back( intersections_to_traverse[0] );
    
    //Keep going until we visited all delivery nodes
    //If we can't find the path to a node, return empty vector since path doesn't exist
    while ( !Set_DeliveryIDs_To_visit.empty() )
    {
        
        std::unordered_set<int>::iterator iter_set = Set_DeliveryIDs_To_visit.find( Order_of_DeliveryIDs.back() );
        
        if ( iter_set != Set_DeliveryIDs_To_visit.end() )
        // Erase it from the set to show that we visited it
        Set_DeliveryIDs_To_visit.erase( iter_set );
        
        
        std::unordered_map<int, vector<Path_and_Time*> >::iterator iter_destination = Hash_Arranged_Destinations.find( Order_of_DeliveryIDs.back()  );
        
        if ( iter_destination != Hash_Arranged_Destinations.end() )
        {
            
        //This will give a vector of Path_and_Time* pointing to the next closest delivery ID
        Path_and_Time* Next_Delivery = iter_destination->second[0];
            
        //See if we already visited this node or not in the set containing the delivery IDs
        iter_set = Set_DeliveryIDs_To_visit.find( Next_Delivery->to_Delivery );
            
            //If we already visited this delivery ID
            //Find the next closest one
            if ( iter_set == Set_DeliveryIDs_To_visit.end() )
            {
                //Keep on searching for a delivery ID that we still didn't visit
                //i starts from 1 because we already checked 0
                for (int i = 1; iter_set == Set_DeliveryIDs_To_visit.end() ; i++)
                {
                    Next_Delivery = iter_destination->second[i];
                    iter_set = Set_DeliveryIDs_To_visit.find( Next_Delivery->to_Delivery );
                }
            }
        
        
        //Add the next delivery ID to the vector of our IDs
        Order_of_DeliveryIDs.push_back( Next_Delivery->to_Delivery );
        
        //cout << "NEXT ID: " << Next_Delivery->to_Delivery << endl;
            
        //Keep on summing up the path time
        Best_Path_Time = Best_Path_Time + Next_Delivery->PathTime;
            
        }
      
        //If this was the last delivery id to visit
        if ( Set_DeliveryIDs_To_visit.size() == 1 )
        
            //Break out of the loop
            break;
    }
    
    
    cout << "Path Time : "<< Best_Path_Time << endl;
    
LessThan15IDs:
    
    //If the number of deliveries are 10 or less
    //Use brute force to find the best path
    //Worst case 10! permutations
   // if ( Order_of_DeliveryIDs.size() < 15 )
    if ( Order_of_DeliveryIDs.size() < 15 )
    {
        //C++ has a built in permuation function
       AllPermutations( Order_of_DeliveryIDs, Best_Path_Time, startTime );
    }
    
    else
    {
    
    /**********************************Optimizer******************************************/
    
    
    //At this point we have a vector<unsigned> of our Order_of_DeliveryIDs, Depot IDs not included!
    //Apply any algorithm to improve this solution
    
    //Keep this a constant number for now, rather than time, so we know the pattern?
     
    //Random 2 Point Swap to get a better solution than greedy algorithm
    Two_Point_Swap ( Order_of_DeliveryIDs, Best_Path_Time, startTime );
    
        srand ( 5 );
    
    clock_t Algorithm_Run_Time = clock();
        
        //Keep on running these algorithms until we time out
        while ( ( ((float) (Algorithm_Run_Time - startTime)) / CLOCKS_PER_SEC ) < 18 )
        {
        
            //2-opt. Every time we get a better solution we call a micro-tuner
            //We call SwapLeftRight_Two
            K_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            
        
            Three_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
    
            Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            
            //This doesn't normall need a time because it is fast and isn't use random #'s
            SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
    
            K_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
    
            Oghab_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime );
    
            //Update time
            Algorithm_Run_Time = clock();
        
            srand( Algorithm_Run_Time );
         }
        
         Oghab_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime );
        
        //After 20 seconds, run Four_Opt_Algorithm
        while ( ( ((float) (Algorithm_Run_Time - startTime)) / CLOCKS_PER_SEC ) < 25 )
        {
            Four_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            
            Oghab_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime );
            
            K_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            
            //Update time
            Algorithm_Run_Time = clock();
            
            srand( Algorithm_Run_Time );
        }
    /*************************************************************************************/
    
    //Oghab_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime );
    
    //Optimize the answer
    //SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
        
    Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
        
    //Permutations and improvements to order of delivery is done
    
    }
    
    
    vector<unsigned> StartPath = {};
    
    //Find the first non-empty path from a depot to the first delviery ID
    //This will be used later on to compare the distance from other depots
    for ( int i = 0; i < depot_locations.size(); i++)
    {
        StartPath = find_path_between_intersections( depot_locations[i], Order_of_DeliveryIDs[0] );
        
        //If the path is not empty(therefore a path exists)
        //Break out of the loop, we found our intial path
        if ( StartPath.size() != 0 )   break;
        
    }
    
    //If we went through all the depot locations and we didnt find a path from a depot to the starting location
    //Then return empty vector show that no courier path exists
    if ( StartPath.size() == 0 )   return vector<unsigned>();
    
    
    vector<unsigned> EndPath = {};
    
    
    //Find the first non-empty path from the last delivery ID to a depot
    //This will be used later on to compare the distance from other depots
    for ( int i = 0; i < depot_locations.size(); i++)
    {
        EndPath = find_path_between_intersections( Order_of_DeliveryIDs.back() , depot_locations[i] );
        
        //If the path is not empty(therefore a path exists)
        //Break out of the loop, we found our intial path
        if ( EndPath.size() != 0 )   break;
        
    }
    
    //If we went through all the depot locations and we didnt find a path from  the last delviery location to a depot
    //Then return empty vector show that no courier path exists
    if ( EndPath.size() == 0 )   return vector<unsigned>();
    
    
    
    //At this point we have both a starting and ending path a depot
    //And this path is non-zero
    //Find the times for these paths
    double StartPathTime = compute_path_travel_time( StartPath );
    double EndPathTime = compute_path_travel_time( EndPath );
    
    
    //Go through all the depots to see if we found a faster path
    //for the intial and final paths
    for ( int i = 0; i < depot_locations.size(); i++)
    {
        //To see if a shorter route exists from a depot to the first delivery ID
        vector<unsigned> newpath = find_path_between_intersections(depot_locations[i], Order_of_DeliveryIDs[0] );
        
        //Check the time only if the vector is not empty
        if ( newpath.size() != 0 )
        {
            
        double newtime = compute_path_travel_time( newpath );
        
        if (newtime < StartPathTime)
        {
            StartPathTime = newtime;
            StartPath = newpath;
        }
            
        }
        
        
        //To see if a shorter route exists from the last delivery ID to any depot
        newpath = find_path_between_intersections( Order_of_DeliveryIDs.back() , depot_locations[i] );
        
        //Check the time only if the vector is not empty
        if ( newpath.size() != 0 )
        {
            
        double newtime = compute_path_travel_time( newpath );
        
        if (newtime < EndPathTime )
        {
            EndPathTime = newtime;
            EndPath = newpath;
        }
            
        }
        
    }
    
    
    
    //Find the vector of street segments to return
    vector<unsigned> Courier_Path;
    
    

    //Find closest depot to the first delivery
    std::unordered_map<int,int>::iterator iter_closest_Depot = Hash_Closest_DepotIDs.find( Order_of_DeliveryIDs[0] );
    
    //Find path from starting depot to first delivery
    vector<unsigned> start_path = find_path_between_intersections( iter_closest_Depot->second, Order_of_DeliveryIDs[0] );
    
    double temp_time =  compute_path_travel_time(start_path);
    
    if ( temp_time < StartPathTime && temp_time != 0 )
        start_path = StartPath;
    
    
    //Insert initial path from depot to first delivery
    Courier_Path.insert( Courier_Path.end(), start_path.begin(), start_path.end() );
    
    for (int i = 1; i < Order_of_DeliveryIDs.size() ; i++)
    {
        
        std::unordered_map<int, std::unordered_map<int, Path_and_Time> >::iterator iter_Path_Time = PreComputed_Paths.find(Order_of_DeliveryIDs[i-1]);
        
        std::unordered_map<int,Path_and_Time>::iterator iter_inner_hashtable = (iter_Path_Time->second).find( Order_of_DeliveryIDs[i] );
        
        
        Courier_Path.insert( Courier_Path.end(), ((iter_inner_hashtable->second).Path).begin(),  ((iter_inner_hashtable->second).Path).end() );
    }
    
    //Find closest geometric depot to last delivery ID
    iter_closest_Depot = Hash_Closest_DepotIDs.find( Order_of_DeliveryIDs.back() );

    //Find path from the last delivery to the closest depot
    start_path = find_path_between_intersections( Order_of_DeliveryIDs.back() , iter_closest_Depot->second );
    
    temp_time =  compute_path_travel_time(start_path);
    
    if ( temp_time < EndPathTime && temp_time != 0 )
        start_path = EndPath;

    
    Courier_Path.insert( Courier_Path.end(), start_path.begin(), start_path.end() );
    
    
    
    //Time taken for this function to execute
    clock_t currentTime = clock ();
    float timeSecs = ((float) (currentTime - startTime)) / CLOCKS_PER_SEC;
    cout << "TIME TAKEN For M4 Function: " << timeSecs << endl;
    
 
    
    //return The_Courier_Path;
    return Courier_Path;
    
 
}










//Using brute force only if the number of deliveries is 10 or less
void AllPermutations( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    
    clock_t Curr_Time = clock();
    
    //You have to sort it in ascending order before using the built in permutation function
    sort( DeliveryOrder_Testing.begin(), DeliveryOrder_Testing.end() );
        
        do
        {
            //Now calculate the time of this new order of deliveries to see if it is better
            double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path By ALL PERMUATIONS" << endl;
                cout << Best_Path_Time << endl;
            }
            
          //Built in c++ function in <algorithm>
        } while ( next_permutation( DeliveryOrder_Testing.begin()  , DeliveryOrder_Testing.end() ) );
        
    
    
    cout << "No Better Solution by ALL PERMUATIONS" << endl;
    
}



//Returns one if a better solution was found
//Run this function for 0.05 seconds
bool Two_Point_Swap ( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    int rand_num_1, rand_num_2;
    
    int num_pt_swaps = 0;
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    
    
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 0.05 )
    {
    
    //Give a number from 0 to Number of deliveries
    //Don't use 0 to 49! because there are different number of testcases
     rand_num_1 = rand() % Num_Deliveries;
     rand_num_2 = rand() % Num_Deliveries;
    
     num_pt_swaps++;
     
    //Check that they are not the same position in the vector
    if( rand_num_1 != rand_num_2 )
    {
        //If they weren't the same position
        //Swap these two indexes in the vector containing the order of delivery IDs
        //Use iterswap which is built in <algorithm>
        iter_swap( DeliveryOrder_Testing.begin() + rand_num_1, DeliveryOrder_Testing.begin() + rand_num_2 );
        
        //Now calculate the time of this new order of deliveries to see if it is better
        double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
        
        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
        if( NewTime < Best_Path_Time )
        {
            //Update Best Path Time
            Best_Path_Time = NewTime;
            
            //Update the order of delivery IDs
            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
            cout << "We Found a Better Path by 2 POINT SWAP" << endl;
            cout << "Swap #: " << num_pt_swaps << endl;
             cout << Best_Path_Time << endl;
        }
        
        else
        {
        //If it is not better, revert back, go to good one
        //DeliveryOrder_Testing = Order_of_DeliveryIDs
        //Making vectors equal takes time so just swap them back
        iter_swap( DeliveryOrder_Testing.begin() + rand_num_1, DeliveryOrder_Testing.begin() + rand_num_2 );
        }
        
    }
        
        
      //Update the current time
      Curr_Time = clock();

        
    }//End while loop (keep running this function until our time is < 28 seconds)
     //So we don't time out
    
    cout << "NUM SWAPS: " << num_pt_swaps << endl;
    
    cout << "No Better Solution by 2 POINT SWAP" << endl;
    
}



//Run Oghab's algortithm for 0.15 seconds
//Oghab's algorithm is only local permutations, not major changes
bool Oghab_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    int rand_num_1, rand_num_2;
    
    int Num_Oghabs = 0;
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    
    
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 0.3 )
    {
        
        //Give a number from 0 to Number of deliveries
        //Don't use 0 to 49! because there are different number of testcases
        rand_num_1 = rand() % Num_Deliveries;
        rand_num_2 = rand() % Num_Deliveries;
        
        //Check that they are not the same position in the vector
        if( rand_num_1 != rand_num_2 )
        {
            //Three situations
            //Check if the numbers are the first and last index of the vector
            
            if ( rand_num_1 == 0 && rand_num_2 == (Num_Deliveries -1) )
            {
                //This case and the one after result in similar swaps in the vector
                goto Same;
            }
            
            //Same as previous if but rand_num_1 and 2 are reversed
            else if ( rand_num_2 == 0 && rand_num_1 == (Num_Deliveries -1) )
            {
                
            Same:
                //Switch the first element and the one after it
                iter_swap( DeliveryOrder_Testing.begin() , DeliveryOrder_Testing.begin() + 1 );
                
                //Swap the last element and the one before it
                //You can't use .back() because iter_swap needs pointers so use .end()
                iter_swap( DeliveryOrder_Testing.end() - 1 , DeliveryOrder_Testing.end() - 2 );
                
                goto SameForAll;
            }
            
            
            else
            {
                //Second number points to the last index
                if ( rand_num_2 == (Num_Deliveries -1) )
                {
                    //Swap index by first random number and its neighbour to the right
                    iter_swap( DeliveryOrder_Testing.begin() + rand_num_1, DeliveryOrder_Testing.begin()+ (rand_num_1 + 1) );
                    
                    //Swap the last element and the one before it
                    iter_swap( DeliveryOrder_Testing.end() - 1 , DeliveryOrder_Testing.end() - 2 );
                    
                    goto SameForAll;
                }
                
                //First number points to the last index
                else if ( rand_num_1 == (Num_Deliveries -1) )
                {
                    
                    //Swap index by second random number and its neighbour to the right
                    iter_swap( DeliveryOrder_Testing.begin() + rand_num_2, DeliveryOrder_Testing.begin()+ (rand_num_2 + 1) );
                    
                    //Swap the last element and the one before it
                    iter_swap( DeliveryOrder_Testing.end() - 1 , DeliveryOrder_Testing.end() - 2 );
                    
                    goto SameForAll;

                }
                
                {
                
                //If they weren't the same position
                //Swap rand_num_1 with the one after it and do the same with rand_num_2
                //Use iterswap which is built in <algorithm>
                iter_swap( DeliveryOrder_Testing.begin() + rand_num_1, DeliveryOrder_Testing.begin()+ (rand_num_1 + 1) );
                
                iter_swap( DeliveryOrder_Testing.begin() + rand_num_2, DeliveryOrder_Testing.begin() + (rand_num_2 + 1) );
                }
            }
            
           
            
        SameForAll:
            
            //Now calculate the time of this new order of deliveries to see if it is better
            double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by OGHAB Algo" << endl;
                cout << "Num Oghab: " << Num_Oghabs << endl;
                cout << Best_Path_Time << endl;
            }
            
            //Revert back to the good one
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
        }
        
        Num_Oghabs++;
        
        //Update the current time
        Curr_Time = clock();
        
    }//End while loop (keep running this function until our time is < 28 seconds)
    //So we don't time out
    
    cout << "Num Oghabs: " << Num_Oghabs << endl;
    cout << "No Better Solution by OGHAB Algo" << endl;
    
}




bool SwapLeftRight_Two( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    
    clock_t Curr_Time = clock();
    
    //Repeat again, a swap that didnt work might now work
repeat:
    
    //Swap with its two left and two right neighbour
    //So start from the second index and end at two before the last index
    for (int i = 2; i < (Num_Deliveries - 2); i++)
    {
        
        //Use iterswap which is built in <algorithm>
        //First swap ith element with its left neighbour
        iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i-1) );
        
        
        //Now calculate the time of this new order of deliveries to see if it is better
        double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
        
        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
        if( NewTime < Best_Path_Time )
        {
            //Update Best Path Time
            Best_Path_Time = NewTime;
            
            //Update the order of delivery IDs
            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
            cout << "We Found a Better Path by SwapLeftRight Two" << endl;
            cout << Best_Path_Time << endl;
            
            //Try swapping again, a swap that didnt work might now work
            goto repeat;
        }
        
        //Else if not better than swap back to orginial
        else
        iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i-1) );
        
        
        //Use iterswap which is built in <algorithm>
        //First swap ith element with its second left neighbour
        iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i-2) );
        
        NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
        
        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
        if( NewTime < Best_Path_Time )
        {
            //Update Best Path Time
            Best_Path_Time = NewTime;
            
            //Update the order of delivery IDs
            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
            cout << "We Found a Better Path by SwapLeftRight Two" << endl;
            cout << Best_Path_Time << endl;
            
            //Try swapping again, a swap that didnt work might now work
            goto repeat;
        }
        
        //Else if not better than swap back to orginial
        else
          iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i-2) );
        
        
        
        //Now swap ith element with its right neighbour
        iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i+1) );
        
        NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
        
        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
        if( NewTime < Best_Path_Time )
        {
            //Update Best Path Time
            Best_Path_Time = NewTime;
            
            //Update the order of delivery IDs
            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
            cout << "We Found a Better Path by SwapLeftRight Two" << endl;
            cout << Best_Path_Time << endl;
            //Try swapping again, a swap that didnt work might now work
            goto repeat;
        }
        
        //Else if not better than swap back to orginial
        else
            iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i+1) );
        
        
        //Now swap ith element with its second right neighbour
        iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i+2) );
        
        NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
        
        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
        if( NewTime < Best_Path_Time )
        {
            //Update Best Path Time
            Best_Path_Time = NewTime;
            
            //Update the order of delivery IDs
            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
            cout << "We Found a Better Path by SwapLeftRight Two" << endl;
            cout << Best_Path_Time << endl;
            //Try swapping again, a swap that didnt work might now work
            goto repeat;
        }
        
        //Else if not better than swap back to orginial
        else
            iter_swap( DeliveryOrder_Testing.begin() + i, DeliveryOrder_Testing.begin() + (i+2) );
    }
    
    //Update the current time
    Curr_Time = clock();
    

    cout << "No Better Solution by SwapLeftRight Two" << endl;
    
}







//2-Opt Algorithm
//Only run this function for 2 seconds at a time
//First part for 1 second and second part for 1 seconds
 bool K_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    int rand_num_1, rand_num_2;

    int k_Opt_Num = 0;
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    
    
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 0.7 )
    {
        
        //Give a number from 0 to ( Number of deliveries - 1)
        //Don't use 0 to 49! because there are different number of testcases
        //Example if there are 50 deliveries, we want a number from 0 to 48
        //Because 2-opt splits the vector of delivery IDs into segments
        rand_num_1 = rand() % ( Num_Deliveries - 1);
        rand_num_2 = rand() % ( Num_Deliveries - 1);
        
        
        //Whatever numbers we get, we split the vector from AFTER the index pointed by the random #s
        
        //Check that they are not the same position in the vector
        if( rand_num_1 != rand_num_2 )
        {
            
            //Make sure that rand_num_1 is the smaller random number
            //If it is bigger, change it to be the smaller one
            if ( rand_num_2 < rand_num_1 )
            {
                int Temp_number = rand_num_1;
                rand_num_1 = rand_num_2;
                rand_num_2 = rand_num_1;
            }
            

            //Swap the segments two and three seperated by the 2 random numbers
            //rotate the vector from AFTER the index pointed by the random #s
            //rotate is in <algorithm> , O(n) complexity
            rotate( DeliveryOrder_Testing.begin() + (rand_num_1 + 1), DeliveryOrder_Testing.begin() +    (rand_num_2 + 1), DeliveryOrder_Testing.end()  );
            
        
            //Now calculate the time of this new order of deliveries to see if it is better
            double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 2-OPT Rotate Segment two and three" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            

        }
        
        
        k_Opt_Num++;
        
        //Update the current time
        Curr_Time = clock();
        
    }//End while loop (keep running this function until our time is < 28 seconds)
    //So we don't time out
    
    
    srand(5);
    //Now second part of K-opt
    
    //Update the current time
    Curr_Time = clock();
    
    //Run for 1.5 seconds
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 1 )
    {
        
        //Give a number from 0 to ( Number of deliveries - 1)
        //Don't use 0 to 49! because there are different number of testcases
        //Example if there are 50 deliveries, we want a number from 0 to 48
        //Because 2-opt splits the vector of delivery IDs into segments
        rand_num_1 = rand() % ( Num_Deliveries - 1);
        rand_num_2 = rand() % ( Num_Deliveries - 1);
        
        
        //Whatever numbers we get, we split the vector from AFTER the index pointed by the random #s
        
        //Check that they are not the same position in the vector
        if( rand_num_1 != rand_num_2 )
        {
            
            //Make sure that rand_num_1 is the smaller random number
            //If it is bigger, change it to be the smaller one
            if ( rand_num_2 < rand_num_1 )
            {
                int Temp_number = rand_num_1;
                rand_num_1 = rand_num_2;
                rand_num_2 = rand_num_1;
            }
            
            
            
                        //First reverse the second piece of the vector
                        //reverse is in <Algorithm>
                        reverse( DeliveryOrder_Testing.begin() + (rand_num_1 + 1), DeliveryOrder_Testing.begin() + (rand_num_2 + 1)  );
            
                        //Now calculate the time of this new order of deliveries to see if it is better
                         double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
                        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                        if( NewTime < Best_Path_Time )
                        {
                            //Update Best Path Time
                            Best_Path_Time = NewTime;
            
                            //Update the order of delivery IDs
                            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
                            cout << "We Found a Better Path by 2-OPT - Reverse Second Piece" << endl;
                            cout << "K_OPT#: " << k_Opt_Num << endl;
                            cout << Best_Path_Time << endl;
            
                            //Call a micro-tuning option here to optimize our new best path!
                            SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                             Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                        }
            
                        else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
                        //Reverse the First Piece
            reverse( DeliveryOrder_Testing.begin(), DeliveryOrder_Testing.begin() + (rand_num_1 + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 2-OPT - Reverse First Piece" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            
                        //Reverse whole delivery to test
                        reverse( DeliveryOrder_Testing.begin() , DeliveryOrder_Testing.end() );
            
                        //Now calculate the time of this new order of deliveries to see if it is better
                        NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
                        //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                        if( NewTime < Best_Path_Time )
                        {
                            //Update Best Path Time
                            Best_Path_Time = NewTime;
            
                            //Update the order of delivery IDs
                            Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
                            cout << "We Found a Better Path by 2-OPT - Reverse WHOLE" << endl;
                            cout << "K_OPT#: " << k_Opt_Num << endl;
                            cout << Best_Path_Time << endl;
            
                            //Call a micro-tuning option here to optimize our new best path!
                            SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                             Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                        }
            
            
                        //If nothing improved it then revert back to original
                        else  DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
        }
        
        
        k_Opt_Num++;
        
        //Update the current time
        Curr_Time = clock();
        
    }//End while loop (keep running this function until our time is < 28 seconds)
    //So we don't time out
    
    
    
    
    cout << "Number of K_Opts: " << k_Opt_Num << endl;
    cout << "No Better Solution by 2-OPT" << endl;
}




//We need three random numbers for this
//run this function for 1 seconds
 void Three_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    
    int k_Opt_Num = 0;
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    
    
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 1.00 )
    {
        
        //Give a number from 0 to ( Number of deliveries - 1)
        //Don't use 0 to 49! because there are different number of testcases
        //Example if there are 50 deliveries, we want a number from 0 to 48
        //Because 2-opt splits the vector of delivery IDs into segments

        vector<int> Random_Nums;
        
        //Get 3 random numbers
        for (int k = 0; k < 3; k++)
        {
             Random_Nums.push_back( rand() % ( Num_Deliveries - 1) );
        }
     
        //Sort them in ascending order to check for uniqueness
        sort( Random_Nums.begin(), Random_Nums.end() );
        
        //Whatever numbers we get, we split the vector from AFTER the index pointed by the random #s
        
        //Check that these random numbers are all unique (not the same)
        //unique is in <algorithm>
        if( unique( Random_Nums.begin(), Random_Nums.end() ) == Random_Nums.end() )
        {
 
            double NewTime;
            
            //Swap segment two and four
            {
                vector<unsigned> First_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
                
                vector<unsigned> Second_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1),  DeliveryOrder_Testing.end()  );
                
                
                
                vector<unsigned> Swapped_Vector(  DeliveryOrder_Testing.begin(),  DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1) );
                
                
                //Add the second segment
                Swapped_Vector.insert( Swapped_Vector.end(), Second_Segment.begin(), Second_Segment.end() );
                
                
                Swapped_Vector.insert( Swapped_Vector.end(), DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1) );
                
                //Add the First segment
                Swapped_Vector.insert( Swapped_Vector.end(), First_Segment.begin(), First_Segment.end() );
                
                
                
                //Now calculate the time of this new order of deliveries to see if it is better
                NewTime = FindTime_FromDeliveryList( Swapped_Vector );
                
                
                //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                if( NewTime < Best_Path_Time )
                {
                    //Update Best Path Time
                    Best_Path_Time = NewTime;
                    
                    //Update the order of delivery IDs
                    Order_of_DeliveryIDs = Swapped_Vector;
                    DeliveryOrder_Testing = Swapped_Vector;
                    
                    cout << "We Found a Better Path by 3-OPT SWITCH seg 2 & 4" << endl;
                    cout << "K_OPT#: " << k_Opt_Num << endl;
                    cout << Best_Path_Time << endl;
                    
                    //Call a micro-tuning option here to optimize our new best path!
                    SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                    Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                }
                
            }
            
            
            
            
            
            //Reverse the segment between the first and second Random numbers INCLUSIVE!
            //Not like my 2-opt algorithm which did it after the index
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0]) , DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT Reverse Betw First & Second" << endl;
                cout << "3_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Reverse the segment between the second and third Random numbers INCLUSIVE!
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[1]) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT Reverse Betw Second & Third" << endl;
                cout << "3_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Rotate segment 3 and 4
            //rotate is in <algorithm> , O(n) complexity
            //rotate( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1), DeliveryOrder_Testing.begin() +    (Random_Nums[2] + 1), DeliveryOrder_Testing.end()  );
            
            
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[2]) , DeliveryOrder_Testing.end() );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT Reverse Betw Third & End" << endl;
                cout << "3_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Reverse the second piece of the vector
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1) , DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT - Reverse second" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            //Reverse the third piece of the vector
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT - Reverse Third" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            //Reverse the segment between the first and third random point Inclusive
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0]) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 3-OPT Reverse Betw First & Third" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                 Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
        }
        
        
        k_Opt_Num++;
        
        //Update the current time
        Curr_Time = clock();
        
    }//End while loop (keep running this function until our time is < 28 seconds)
    //So we don't time out
    
    cout << "Number of 3_Opts: " << k_Opt_Num << endl;
    cout << "No Better Solution by 3-OPT" << endl;

    
}

//This picks two indexes i and k in the vector
//And reverses whatever is in between i and k INCLUSIVE
void Wiki_2_Opt( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
  
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    

   repeat_2_opt:
    for ( int i = 0; i < (Num_Deliveries-1); i++)
    {
        for ( int k = (i+1) ; k < Num_Deliveries; k++)
        {
            //Reverse  whatever is in between i and k INCLUSIVE
            reverse( DeliveryOrder_Testing.begin() + i ,DeliveryOrder_Testing.begin() + (k+1) );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            double NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by Wiki_2_Opt" << endl;
                cout << Best_Path_Time << endl;
                
                goto repeat_2_opt;
            }
            
            else
                //Revert back to the good one
                reverse( DeliveryOrder_Testing.begin() + i ,DeliveryOrder_Testing.begin() + (k+1) );
        }
    }

        //Update the current time
        Curr_Time = clock();
        

    
    cout << "Time for Wiki_2_Opt: " << ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC )<< endl;
    cout << "No Better Solution by Wiki_2_Opt" << endl;
    
}




//We need four random numbers for this
//run this function for 1 seconds
void Four_Opt_Algorithm( vector<unsigned>& Order_of_DeliveryIDs, double& Best_Path_Time, clock_t startTime )
{
    
    
    //Copy the vector first don't want to make changes to orginial one yet
    vector<unsigned> DeliveryOrder_Testing = Order_of_DeliveryIDs;
    int Num_Deliveries = Order_of_DeliveryIDs.size();
    
    
    int k_Opt_Num = 0;
    
    startTime = clock();
    
    clock_t Curr_Time = clock();
    
    
    while( ( ((float) (Curr_Time - startTime)) / CLOCKS_PER_SEC ) < 1.00 )
    {
        
        //Give a number from 0 to ( Number of deliveries - 1)
        //Don't use 0 to 49! because there are different number of testcases
        //Example if there are 50 deliveries, we want a number from 0 to 48
        //Because 2-opt splits the vector of delivery IDs into segments
        
        vector<int> Random_Nums;
        
        //Get 4 random numbers
        for (int k = 0; k < 4; k++)
        {
            Random_Nums.push_back( rand() % ( Num_Deliveries - 1) );
        }
        
        //Sort them in ascending order to check for uniqueness
        sort( Random_Nums.begin(), Random_Nums.end() );
        
        //Whatever numbers we get, we split the vector from AFTER the index pointed by the random #s
        
        //Check that these random numbers are all unique (not the same)
        //unique is in <algorithm>
        if( unique( Random_Nums.begin(), Random_Nums.end() ) == Random_Nums.end() )
        {
            
            double NewTime;
            
            //Swap segments of the vector
            
            //Swap segment two and four
            {
                vector<unsigned> First_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
                
                vector<unsigned> Second_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1)  );
                
                vector<unsigned> Swapped_Vector(  DeliveryOrder_Testing.begin(),  DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1) );
                
                //Making the swapped vector
                
                //Add the second segment
                Swapped_Vector.insert( Swapped_Vector.end(), Second_Segment.begin(), Second_Segment.end() );
                
                
                Swapped_Vector.insert( Swapped_Vector.end(), DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1) );
                
                //Add the First segment
                Swapped_Vector.insert( Swapped_Vector.end(), First_Segment.begin(), First_Segment.end() );
                
                //Add the end of the vector
                Swapped_Vector.insert( Swapped_Vector.end(), DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1),  DeliveryOrder_Testing.end() );
                
                
                //Now calculate the time of this new order of deliveries to see if it is better
                NewTime = FindTime_FromDeliveryList( Swapped_Vector );
                
                
                //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                if( NewTime < Best_Path_Time )
                {
                    //Update Best Path Time
                    Best_Path_Time = NewTime;
                    
                    //Update the order of delivery IDs
                    Order_of_DeliveryIDs = Swapped_Vector;
                    DeliveryOrder_Testing = Swapped_Vector;
                    
                    cout << "We Found a Better Path by 4-OPT Switch seg 2 & 4" << endl;
                    cout << "K_OPT#: " << k_Opt_Num << endl;
                    cout << Best_Path_Time << endl;
                    
                    //Call a micro-tuning option here to optimize our new best path!
                    SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                    Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                    Four_Opt_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                    Oghab_Algorithm( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                }
                
            }
            
            
            
            
            //Swap segment three and four
            {
                vector<unsigned> First_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
                
                vector<unsigned> Second_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1)  );
                
                
                
                vector<unsigned> Swapped_Vector(  DeliveryOrder_Testing.begin(),  DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1) );
                
                
                
                //Add the second segment
                Swapped_Vector.insert( Swapped_Vector.end(), Second_Segment.begin(), Second_Segment.end() );
                
                
                //Add the First segment
                Swapped_Vector.insert( Swapped_Vector.end(), First_Segment.begin(), First_Segment.end() );
                
                //Add the end of the vector
                Swapped_Vector.insert( Swapped_Vector.end(), DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1),  DeliveryOrder_Testing.end() );
                
                
                //Now calculate the time of this new order of deliveries to see if it is better
                NewTime = FindTime_FromDeliveryList( Swapped_Vector );
                
                
                //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                if( NewTime < Best_Path_Time )
                {
                    //Update Best Path Time
                    Best_Path_Time = NewTime;
                    
                    //Update the order of delivery IDs
                    Order_of_DeliveryIDs = Swapped_Vector;
                    DeliveryOrder_Testing = Swapped_Vector;
                    
                    cout << "We Found a Better Path by 4-OPT Switch seg 3 & 4" << endl;
                    cout << "K_OPT#: " << k_Opt_Num << endl;
                    cout << Best_Path_Time << endl;
                    
                    //Call a micro-tuning option here to optimize our new best path!
                    SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                    Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                }
                
            }
            
            
            
            
            //Swap segment three and five
            {
                vector<unsigned> First_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
                
                vector<unsigned> Second_Segment( DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1),  DeliveryOrder_Testing.end() );
                
                
                
                vector<unsigned> Swapped_Vector(  DeliveryOrder_Testing.begin(),  DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1) );
                
                
                
                //Add the second segment
                Swapped_Vector.insert( Swapped_Vector.end(), Second_Segment.begin(), Second_Segment.end() );
                
                
                Swapped_Vector.insert( Swapped_Vector.end(), DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1),  DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1) );
                
                
                //Add the First segment
                Swapped_Vector.insert( Swapped_Vector.end(), First_Segment.begin(), First_Segment.end() );
                
                
                //Now calculate the time of this new order of deliveries to see if it is better
                NewTime = FindTime_FromDeliveryList( Swapped_Vector );
                
                
                //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
                if( NewTime < Best_Path_Time )
                {
                    //Update Best Path Time
                    Best_Path_Time = NewTime;
                    
                    //Update the order of delivery IDs
                    Order_of_DeliveryIDs = Swapped_Vector;
                    DeliveryOrder_Testing = Swapped_Vector;
                    
                    cout << "We Found a Better Path by 4-OPT Switch seg 3 & 5" << endl;
                    cout << "K_OPT#: " << k_Opt_Num << endl;
                    cout << Best_Path_Time << endl;
                    
                    //Call a micro-tuning option here to optimize our new best path!
                    SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                    Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
                }
                
            }
            
            
            //Reverse the segment between the first and second Random numbers INCLUSIVE!
            //Not like my 2-opt algorithm which did it after the index
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0]) , DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT Reverse Betw First & Second" << endl;
                cout << "4_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            //Reverse the segment between the Second and Fourth Random numbers INCLUSIVE!
            //Not like my 2-opt algorithm which did it after the index
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[1]) , DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
             NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT Reverse Betw Second & Fourth" << endl;
                cout << "4_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Reverse the segment between the second and third Random numbers INCLUSIVE!
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[1]) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT Reverse Betw Second & Third" << endl;
                cout << "3_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Rotate segment 3 and 4
            //rotate is in <algorithm> , O(n) complexity
            //rotate( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1), DeliveryOrder_Testing.begin() +    (Random_Nums[2] + 1), DeliveryOrder_Testing.end()  );
            
            
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[2]) , DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1) );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
            
                cout << "We Found a Better Path by 4-OPT Reverse Betw Third & Fourth" << endl;
                cout << "3_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
            //Reverse the second piece of the vector
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0] + 1) , DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT - Reverse second" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            //Reverse the third piece of the vector
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[1] + 1) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT - Reverse Third" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            //Reverse the Fourth piece of the vector
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1) , DeliveryOrder_Testing.begin() + (Random_Nums[3] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT - Reverse Fourth" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            //Reverse the segment between the first and third random point Inclusive
            //reverse is in <Algorithm>
            reverse( DeliveryOrder_Testing.begin() + (Random_Nums[0]) , DeliveryOrder_Testing.begin() + (Random_Nums[2] + 1)  );
            
            //Now calculate the time of this new order of deliveries to see if it is better
            NewTime = FindTime_FromDeliveryList( DeliveryOrder_Testing );
            
            
            //IF it is better than the previous saved time then update the vec Order_of_DeliveryIDs
            if( NewTime < Best_Path_Time )
            {
                //Update Best Path Time
                Best_Path_Time = NewTime;
                
                //Update the order of delivery IDs
                Order_of_DeliveryIDs = DeliveryOrder_Testing;
                
                cout << "We Found a Better Path by 4-OPT Reverse Betw First & Third" << endl;
                cout << "K_OPT#: " << k_Opt_Num << endl;
                cout << Best_Path_Time << endl;
                
                //Call a micro-tuning option here to optimize our new best path!
                SwapLeftRight_Two( Order_of_DeliveryIDs, Best_Path_Time, startTime );
                Wiki_2_Opt( Order_of_DeliveryIDs, Best_Path_Time, startTime);
            }
            
            else DeliveryOrder_Testing = Order_of_DeliveryIDs;
            
            
            
        }
        
        
        k_Opt_Num++;
        
        //Update the current time
        Curr_Time = clock();
        
    }//End while loop (keep running this function until our time is < 28 seconds)
    //So we don't time out
    
    cout << "Number of 4_Opts: " << k_Opt_Num << endl;
    cout << "No Better Solution by 4-OPT" << endl;
    
    
}








//This function finds the time of the path given a vector<unsigned> of delivery IDs
//Pass the vector by reference to avoid copying the vector
//Use this function mainly to compute whole delivery time when we do K-opt
double FindTime_FromDeliveryList ( vector<unsigned>& DeliveryIDs  )
{
    double WholePathTime = 0;
    
    for (int i = 1; i < DeliveryIDs.size() ; i++)
    {
        std::unordered_map<int, std::unordered_map<int, Path_and_Time> >::iterator iter_Path_Time = PreComputed_Paths.find( DeliveryIDs[i-1] );
        
        std::unordered_map<int,Path_and_Time>::iterator iter_inner_hashtable = (iter_Path_Time->second).find( DeliveryIDs[i] );
        
        
        WholePathTime = WholePathTime + (iter_inner_hashtable->second).PathTime;
    }
    
    return WholePathTime;
}




/***********************************************************************************************/

//Dijkstra's algorithm to expand from one intersection to all the other 49 intersections
//And leave bread crumbs to trace the path back
//We call this 50 times to precompute all the paths and store them
//Parameters are a pointer to the starting IntersectionNode object in Hash_IntersectionNode_Info and the vector of the 50 intersection IDs
void Dijkstra_M4( IntersectionNode* SourceNode, vector<unsigned>& intersections_to_traverse  )
{
    
    
    //This will be a minimum heap containing all the WaveElements we have to inspect
    //A WaveElement contains
    //1)A pointer to the IntersectionNode to be inspected
    //2)The StreetSegment ID used to reach this node
    //The WaveFront is now stored as a minHeap
    //The minimum WaveElement is stored at the top
    //*DONT use "comparator_WaveElement" because that organizes the heap based on
    //Heuristic_And_TimeFromInitial but we only want it organized based on Travel_Time
    //Because we are doing only Dijkstra's
    priority_queue< WaveElement, std::vector<WaveElement>,comparator_WaveElement_Dijkstra> WaveFront;
    
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
    
    
    //This set contains all the delivery IDs we have to visit
    //Unordered_set is similar to a hash table with no values, only keys
    //Constructed from the vector they gave us in m4 function
    std::unordered_set<int> Set_DeliveryIDs_To_visit( intersections_to_traverse.begin(), intersections_to_traverse.end() );
    
    //Keep on inspecting the WaveElements in the WaveFront list until its empty
    while ( WaveFront.size() != 0 )
    {
        //cout << WaveFront.size() << endl;
        
        //Dijkstra's algorithm: Inspect the top element in the heap first [The minimum WaveElement]
        WaveElement Current_Element_Inspected = WaveFront.top();
        
        //Remove it from the list
        WaveFront.pop();
        
        //If we visited all our delivery intersections, then return
        //Every time we visit one of our destinations, we erase it from the set
        if ( Set_DeliveryIDs_To_visit.empty() )
                goto Done;

        
        Travel_Time_Dropped = ( Current_Element_Inspected.Travel_Time <  (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source );
        
        if ( (Current_Element_Inspected.Node_ptr)->visited == 0 || Travel_Time_Dropped )
        {
            //Update the StSeg ID used to reach this node in the IntersectionNode object in Hash_IntersectionNode_Info
            (Current_Element_Inspected.Node_ptr)->SegID_Reaching_This_Node = Current_Element_Inspected.SegID_reaching_this_node;
            
            //It hasn't been visited before so set the travel time for this node
            (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source = Current_Element_Inspected.Travel_Time;
            
            
            //See if the Current_Element_Inspected is one of the delivery nodes
            //look up in the set/hash table, O(1), really fast
            std::unordered_set<int>::iterator iter_set = Set_DeliveryIDs_To_visit.find( (Current_Element_Inspected.Node_ptr)->IntersectionID );
            
            //If Current_Element_Inspected is one of the delivery nodes
            if (  iter_set != Set_DeliveryIDs_To_visit.end()  )
            {
                //Erase it from the set to indicate that we visited it
                //We might visit it again, its okay, we just want to show we visited it at least once
                //Keep on searching
                Set_DeliveryIDs_To_visit.erase( iter_set );
                
                //Put path_exists as 1 to indicate that at lease ONE path exists to this node
                (Current_Element_Inspected.Node_ptr)->Path_Exists_to_Node = 1;
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
                
     
                //add the time it takes from the source node until this node
                {
                    //Search all the information for this street segment in Hash_StreetSegment_Info
                    unordered_map<int,StreetSegment>::iterator iter2 = Hash_StreetSegment_Info.find(StSegID_to_ReachNode );
                    
                    //Time to traverse the connected street segment
                    //time_from_source = (iter2->second).StreetSegmentLength / ( (iter2->second).StreetSegment_SpeedLimit * KMPH_TO_METREperMIN );
                    
                    //Precomputed in load_map
                    time_from_source = (iter2->second).T_from_source;
                    
                    //To get the total time from source, add the travel time stored in the previous node
                    time_from_source = time_from_source + (Current_Element_Inspected.Node_ptr)->Travel_Time_from_Source;
                    
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
                
                WaveFront.push( WaveElement( &(iter1->second),StSegID_to_ReachNode,time_from_source ) );
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
    
    
Done:
    
    //Copy the nodes visited in global variable
    Intersections_Examined_DuringSearch = Nodes_Examined;
    
    
}





























