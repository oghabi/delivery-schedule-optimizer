#pragma once //protects against multiple inclusions of this header file
#include "m2.h"
#include "m3.h"
#include "m4.h"
#include "path_verify.h"

using namespace std;

/**********************Global Variables*******************************/

//Stores the individual characters passed on from the keyboard while the user is typing
vector<char> User_Input;

//This is the intersection name entered by the user
//If the Intersection entered was valid/found, then we store it in this global variable
string Intersection_Name_Entered;

//When the user enters an intersection, they can save the intersection entered (if Found) by clicking
//the button "Add Intersection"
//This will add the valid intersection into this vector
//When the user wants to find a path between two intersections, the LAST two intersections stored in
//this vector will be considered
vector<string> Intersections_Saved;

//The intial and final IDs of the route shown
int Initial_ID, Final_ID;

//This is the buffer used by the readline() function for command parsing
char* Buffer_UserInput;

//This buffer is const char* because it is used to point to a string
//and a string is a const char*
//We CANNOT free this buffer!
const char* Buffer_UserInput_1;

//Stores the LatLon coordinate of the point clicked using the mouse
LatLon LatLon_Clicked_By_User;

//Stores the (x,y) coordinate of the point clicked using the mouse
t_point XY_Clicked_By_User;

//Contains all the clicks by the user
//We can use these clicks to find the path between the last two clicks
vector<LatLon> Click_Coordinates;

string POI_Entered;


//Used in draw_screen()
//When the user presses enter, if the entered intersection is found
//Then draw a red dot at that intersection. This boolean is used to draw the red dot.
bool IntersectionEntered_Found = 0;
int  IntersectionEntered_ID;

//Used for the button "POIs ON"
//When it is clicked, it turns all the POIs on
bool AllPOIsOn = 0;

//Used for the button "Features ON"
//When it is clicked, it turns all the Features on
bool AllFeaturesOn = 0;

//Used for our special feature
//When the user clicks on a point in the map, Show all the POIs within 1Km of that point clicked
//Will later change the range so the user can specify his/her own range
bool PointClicked = 0;

bool POI_Found = 0;

/********************************************************************************/

 //vector<unsigned> temp1 =     {334, 4904, 6868, 9693, 11584, 16073, 19450, 22197, 23666, 24352, 24756, 24837, 26373, 28349, 28760, 29864, 33378, 44271, 45997, 53528, 54101, 59155, 59257, 59823, 63019, 67387, 68370, 69112, 71050, 71395, 75438, 85427, 86482, 97085, 103839, 105650, 109689, 119610, 122014, 129738, 137740, 140247, 141616, 143665, 143872, 144521, 152618, 157705, 159225, 163352};

 vector<unsigned> temp1 =  {334, 4904, 6868, 9693, 11584, 16073, 19450, 22197, 23666, 24352, 24756, 24837, 26373, 28349, 28760, 29864, 33378, 44271, 45997, 53528, 54101, 59155, 59257, 59823, 63019, 67387, 68370, 69112, 71050, 71395, 75438, 85427, 86482, 97085, 103839, 105650, 109689, 119610, 122014, 129738, 137740, 140247, 141616, 143665, 143872, 144521, 152618, 157705, 159225, 163352};
 vector<unsigned> temp2 = {6604, 15321, 30063, 57131, 77774, 97918, 102219, 103141, 160373, 160522};


// Draws the map whose at map_path; this should be a .bin file.
void draw_map(std::string map_path)
{
    
    load_map(map_path);

    
    /**************** initialize display **********************/
    // Set the name of the window (in UTF-8), with the background.
    init_graphics("ECE297 Map", Google_Background);
    
    
   //Get the (x,y) coordinates of the minimum and maximum points of the entered map
   //Convert the LatLon points to (x,y) points
    t_point Min_XY_of_Map ( Convert_LatLon_to_XY(MinimumLatLon_ofMap) );
    t_point Max_XY_of_Map ( Convert_LatLon_to_XY(MaximumLatLon_ofMap) );
    
    
   // Set-up coordinate system. The coordinate system used is a normal (x-y) axis
   set_visible_world( Min_XY_of_Map.x, Min_XY_of_Map.y, Max_XY_of_Map.x, Max_XY_of_Map.y );
    
    //Create Extra Buttons
    create_button("Window", "X", undo_button);
    create_button("X", "Add Intersection"  ,Add_Intersection);
    create_button("Add Intersection","Route Int-Int"  ,Route_2_Intersection);
    create_button("Route Int-Int","Route pt-pt"  ,Route_2_Points);
    create_button("Route pt-pt","Route Int-POI"  ,Route_2_POI);
    create_button("Route Int-POI","Print Directions"  ,Print_Directions);
    create_button("Print Directions", "Courier"  ,Courier);
    create_button("Courier", "POI range"  ,POI_Range);
    create_button("POI range", "POIs ON"  ,All_POIs_On);
    create_button("POIs ON", "Features ON"  ,All_Features_On);
    
    
    
    // Enable mouse movement (not just button presses) and key board input.
    set_keypress_input (true);
    set_mouse_move_input (true);

    
    
 
  //   clock_t TimeStart = clock();
    The_Path_For_Drawing = traveling_salesman( temp1, temp2 );
  //  clock_t TimeEnd = clock();
 //    float timeSecs = ((float) (TimeEnd - TimeStart)) / CLOCKS_PER_SEC;
   // cout << "TIME: " << timeSecs << endl;
//    
//    std::unordered_map<int, std::unordered_map<int, Path_and_Time>  >::iterator iter5 = PreComputed_Paths.find( temp1[6] );
////    
////    
//    std::unordered_map<int,Path_and_Time>::iterator iter4 = (iter5->second).find( temp1[11] );
//    
//    cout <<  (iter4->second).PathTime << "  |  " ;
//    
//     cout <<  compute_path_travel_time_ofDeque( (iter4->second).Path )<< "  |  " ;
//    
//    clock_t TimeStart = clock();
 
  //  The_Path_For_Drawing = find_path_between_intersections( temp1[6], temp1[11] );
    
//    clock_t TimeEnd = clock();
//    
//    float timeSecs = ((float) (TimeEnd - TimeStart)) / CLOCKS_PER_SEC;
//    
//   // cout << "TIME TAKEN A*: " << timeSecs << endl;
   // cout << compute_path_travel_time( The_Path_For_Drawing ) << endl;
    

    
    
    // Pass control to the window handling routine.
    //All 4 call back functions are passed in
    
    
    event_loop(act_on_button_press, act_on_mouse_move, act_on_key_press, drawscreen);
    
    
    
    //NOTE: BEFORE THE USER ENTERS ANOTHER MAP, CLEAR ALL THE GLOBAL VARIABLES I MADE
    //ie. Go through all the global hash tables & vectors I have and erase all their entries
    //If you don't clear their entries, then when the user selects another map, it won't draw!
    //Cuz the Hash Tables are already full
    Clear_Global_Variables();
    
    close_graphics();
}


/* Called whenever event_loop gets a button press in the graphics *
 * area.  Allows the user to do whatever he/she wants with button *
 * clicks.                                                        */
//Store all the clicks in a vector Click_Coordinates
void act_on_button_press (float x, float y, t_event_buttonPressed event)
{
   
    //Convert the (x,y) points to LatLon
    //Use it to compute the POIs within 1Km of this point
    LatLon_Clicked_By_User = Convert_XY_to_LatLon(x, y);
    
    
    //Store the values into a global variable of type t_point
    XY_Clicked_By_User.x = x;
    XY_Clicked_By_User.y = y;
    
    
    //Add the coordinates of the location pressed into the vector Click_Coordinates
    Click_Coordinates.push_back( LatLon_Clicked_By_User );
    
}



void drawscreen()
{
    /* Should precede drawing for all drawscreens */
    //For zooming and panning
    clearscreen();
    
    
    //Get the lower left and upper right bounds of the screen
    //everytime the user zooms or pans the map
    //UICoords = User Interface coordinates
    t_bound_box UICoords = get_visible_world();
    
    float UI_Area = UICoords.area();
    
    
   //Use this for testing
   // cout << UICoords.area() << endl;
    
    
    

    
    /*********************SHOW DELIVERY IDS AS GREEN*************************************/
    setcolor (RED);
    
    //Go through all the POIs of the map to draw a red dot on its location
    for (int i = 0; i < temp1.size(); i++)
    {
        
        //The LatLon of each POI
        LatLon POI_Location = getIntersectionPosition( temp1[i] );
        

        
            t_point XY_of_POI = Convert_LatLon_to_XY(POI_Location);
        
            fillarc(XY_of_POI, 0.00005, 0, 360);
        
    
    }
    
    
    setcolor (BLUE);
    
    //Go through all the POIs of the map to draw a red dot on its location
    for (int i = 0; i < temp2.size(); i++)
    {
        
        //The LatLon of each POI
        LatLon POI_Location = getIntersectionPosition( temp2[i] );
        
        if ( temp2[i] ==  160522)
        {
            setcolor(BLUE);
            t_point XY_of_POI = Convert_LatLon_to_XY(POI_Location);
            
            fillarc(XY_of_POI, 0.00006, 0, 360);
            setcolor (BLUE);

        }
        
        else
        {
        t_point XY_of_POI = Convert_LatLon_to_XY(POI_Location);
        
        fillarc(XY_of_POI, 0.00006, 0, 360);
        }
        
        
    }
    
    /*********************************************************************************/
    

//DRAW POLYGONS FOR CLOSED FEATURES & JOINT LINES FOR OPEN FEATURES
/*********************************Draw The Features*****************************/
if (AllFeaturesOn)
{
    
    Draw_Features();
    
}
/********************************************************************************************/
    
    
    //Always Draw streets on top of features
    setlinestyle (SOLID);
    
    
    //Draw the Streets
    Draw_Streets(UI_Area);
    

/*****************Used to Show the Entered Intersection with a Red Dot**************************/
    //Change this as zoom out and zooming in levels decrease and increase
    //Constant for right now
    float radius = 0.000005;
    
    //If the entered intersection by the user is found
    //Draw a red dot
    if ( IntersectionEntered_Found )
    {
        //Get the ID of the interesction searched
        //Get the LatLon of that ID
        LatLon Intersection_Entered = getIntersectionPosition( IntersectionEntered_ID );
        
        
        
        //Convert the LatLon to (x,y) coordinates using function in m1.h
        t_point XY_Intersection = Convert_LatLon_to_XY( Intersection_Entered );
        
        setcolor (RED);
        
        if( UICoords.area() < 6.80913e-07  )
        {
            fillarc(XY_Intersection, 0.00001, 0, 360);
        }
        
        else
        //Draw a very small red dot on the intersection entered
        fillarc(XY_Intersection, 0.00003, 0, 360);
    }
    
    

    
/********************************************************************************************/
    
    
/**********If The button was clicked, show all POIs on the map with red dots****************/
    if ( AllPOIsOn )
    
    {
        
        //Go through all the POIs of the map to draw a red dot on its location
        for (int i = 0; i < getNumberOfPointsOfInterest(); i++)
        {
            //The LatLon of each POI
            LatLon POI_Location = getPointOfInterestPosition( i );
            
            //Convert the LatLon to (x,y) coordinates using function in m1.h
            t_point XY_POI = Convert_LatLon_to_XY( POI_Location );
            
            setcolor (RED);
            
            
            //Use level of detail functions because once we zoom in past a certain point
            //The red dots occupy too much space on the map!
            
            //Use 3 levels of zooming
            
            //If ZOOMED IN past a point, draw the red dots smaller
            if( UICoords.area() < 1.8889e-06  )
            {
                fillarc(XY_POI, 0.000003, 0, 360);
            }
            
            else if( UICoords.area() < 6.80913e-07  )
            {
                fillarc(XY_POI, 0.0000003, 0, 360);
            }
            
            else
                
            //Draw a very small red dot on the POI location
            fillarc(XY_POI, radius, 0, 360);
        }
        
        
    }
    
/********************************************************************************************/
    
    
/*******User Clicked a Point on map, show all POIs within range of clicked point***************/
    /***********************************SPECIAL FEATURE******************************/
    if (PointClicked)
    {
        //Draw a dotted circle centered at the Point Clicked to show the range (1Km right now)
        setlinestyle(DASHED);
        setcolor (RED);
        
        //Make the line slightly thicker
        setlinewidth (2);
        
        //The radius is 1km, but divide by EARTH_RADIUS_IN_METERS
        //To scale it on the map
        drawarc(XY_Clicked_By_User.x, XY_Clicked_By_User.y, 1000/EARTH_RADIUS_IN_METERS, 0, 360);
        
        
        
        
        //Go through all the POIs of the map to draw a red dot on its location
        for (int i = 0; i < getNumberOfPointsOfInterest(); i++)
        {
            
            //The LatLon of each POI
            LatLon POI_Location = getPointOfInterestPosition( i );
            
            //Find the distance between each POI and the point clicked
            //If the distance is less than 1000 (1Km) draw the POI
            if ( find_distance_between_two_points( LatLon_Clicked_By_User, POI_Location ) < 1000 )
            
            {
                t_point XY_of_POI = Convert_LatLon_to_XY(POI_Location);
                
                if( UICoords.area() < 1.8889e-06  )
                {
                    fillarc(XY_of_POI, 0.000003, 0, 360);
                }
                
                else if( UICoords.area() < 6.80913e-07  )
                {
                    fillarc(XY_of_POI, 0.0000009, 0, 360);
                }
                
                else
                    
                  fillarc(XY_of_POI, 0.000006, 0, 360);
                
            }
            
        }
        
        
        //Drawing done
        //Set back to Solid for other drawing aspects
        setlinestyle(SOLID);
    }
    
}



void act_on_key_press (char c)
{
    // function to handle keyboard press event, the ASCII character is returned
    
    //if user presses "BackSpace" then remove one character from the end of the vector<char> User_Input
    //Backspace = '\b'
    if (c == '\b')
    {
        //Check to see if the vector is not empty
        //If it is empty, erasing will cause the program to crash
        
        //If there ARE any characters in the vector then erase them
        if (User_Input.size() != 0)
        User_Input.erase( User_Input.end() - 1 );
        
    }
    
    //if user enters "enter/return" key, then search for the intersection name entered by user
    //Enter = Carriage Return (cr) = '\r'
    else if (c == '\r')
        
            goto MustDo;
    
    //Else add the character entered into the vector<char>
    //If keys like "shift" are pressed, don't add them to the vector<char> because it messes the string when you want to search for an Intersection name
    else if (c != '\0')
    User_Input.push_back(c);
    
    
    //These are the Must Do statements that must be executed no matter what character user entered
    MustDo:
    
   
    //Combines all the characters in the vector to form a string
    string output( User_Input.begin() , User_Input.end() );
    
    //Print that string below the user interface
    update_message(output);
    
    
    //These check if the arrow keys were pressed
    //We only care about up arrow key, in case the user wants to check the history of anything
    //they entered
    if ( Arrow_Key_Pressed == UP_ARROW )
    {
        //When the user presses the UP arrow, we switch over to the terminal
        //The has to press UP AGAIN for a second time to view the history of the things entered
        
        //Use tab for auto completion
        rl_bind_key('\t', rl_complete);
        //Use our function for auto-complete
        rl_attempted_completion_function = command_completion;
        //Tell readline to handle double and single quotes for us
        rl_completer_quote_characters = strdup("\"\'");
        
        
        //Dont use while loop, only once
        Buffer_UserInput = readline(" Enter Here: ");
       
        //Convert char* (pointer to characters(array)) to a string
        //Built in string constructor
        string temp( Buffer_UserInput );
        
        output = temp;
        
        //After we are done with the buffer, free it
        free(Buffer_UserInput);
        
        //Set it to Null
        Buffer_UserInput = NULL;
        
        //Check if the entered intersection name exists
        goto search;
        
    }
    
    
    
    //If user presses enter/return key
    //Then search for the intersection entered
    if (c == '\r')
    {
        //When the user presses enter
        //Add the entered name to list of the buffer history (no matter if it is not valid)
        
        //Make this const char* buffer point to string output of what the user typed
        //.c_str() converts a string to a char*
        Buffer_UserInput_1 = output.c_str();
        
        //Only add this name to the history if it is not empty
        if( strcmp (Buffer_UserInput_1, "" ) != 0 )
        add_history(Buffer_UserInput_1);
        
        
        //Searching for the entered intersection
    search:
        //if it doesn't exist, then that function returns -666 by our convention
        int foundID = find_intersection_id_from_name( output );
        
        //If entered intersection wasn't found, then print a message for user on GUI
        if ( foundID == -666 )
        {
            
            //Maybe the user selected a POI and not an intersection name!
            //Search if this POI name exists
            //We already have a hash table "Hash_POI_IDs" where we store all the POI names
            std::unordered_map<string,vector<unsigned> >::iterator iter = Hash_POI_IDs.find(output);
            
            
            //If we didn't find it
            //Print "NOT FOUND"
            if ( iter ==  Hash_POI_IDs.end() )
            {
                update_message("NOT FOUND!");
                IntersectionEntered_Found = 0;
            }
            
            //We found it
            else
            {
                //This is a global variable to store the name of the POI Entered
                POI_Entered = output;
                POI_Found = 1;
                //We found it
                update_message("FOUND!");
            }
            
            
        }
        
        else
        {
            update_message("FOUND!");
            
            //Store the intersection name entered in this global variable
            Intersection_Name_Entered = output;
            
            //Add this intersection name entered to be stored in the readline buffer
            //In case the user wants to press the "UP" arrow key and check the history
            
            IntersectionEntered_Found = 1;
            IntersectionEntered_ID = foundID;
        
            //Get the ID of the interesction searched
            //Get the LatLon of that ID
            LatLon Intersection_Entered = getIntersectionPosition( IntersectionEntered_ID );
            
            //Convert the LatLon to (x,y) coordinates using function in m1.h
            t_point XY_Intersection = Convert_LatLon_to_XY( Intersection_Entered );
            
            setcolor (RED);
            //Draw a very small red dot on the intersection entered
            fillarc(XY_Intersection, 0.000006, 0, 360);
        }
    }
    
    
    //Reset this back to zero
    Arrow_Key_Pressed = 0;
    
}


void act_on_mouse_move (float x, float y)
{
    
}




/****************************************THE BUTTONS*****************************************/

// "X" button
// This button is used in case you make any mistakes. You can press it and simply start over again
//Should clear everything
// 1) Clear the vector<char> to erase anything the user inputted
// 2) Set IntersectionEntered_Found to false
// 3) Set AllPOIsOn to false again to removed the red dots on the map
// 5) Set PointClicked to false again to stop drawing the POIs in the range of the cliked point

void undo_button ( void (*drawscreen_ptr) (void)  )
{
    
    //Change the button back to its original position
    if ( AllPOIsOn == 1)
    {
        change_button_text( "POIs OFF", "POIs ON");
        AllPOIsOn = 0;
    }
    
    
    
    //1) Emptying the vector<char> filled with the characters the user entered
    vector<char>::iterator iter = User_Input.begin();
    
    while (iter != User_Input.end())
    {
        iter = User_Input.erase(iter);
    }
    
    //Clear whatever was written before
    update_message("");
    
    
    // 2)Set IntersectionEntered_Found to false
    // Because you don't want to keep drawing red dots everywhere
    // When user presses "x", clear the screen
    IntersectionEntered_Found = 0;
    
    POI_Found = 0;

    // 3)Set AllPOIsOn to false again to removed the red dots on the map
    //   When the user presses "X" button in the GUI
    AllPOIsOn = 0;
    
    
    // 5) Set PointClicked to false again to stop drawing the POIs in the range of the cliked point
    PointClicked = 0;
    
    
    //Erase the path so it doesn't draw
    vector<unsigned> temp = {};
    
    //Empty the path
    The_Path_For_Drawing = temp;
    
    //Draw Screen again after undoing everything
    drawscreen();
    
}



//"POIs ON" button
//Shows all the POIs on the map as red dots
void All_POIs_On ( void (*drawscreen_ptr) (void)  )
{
    //This if else statement is used to only use one button to
    //turn the POIs on and off
    //No need to interact with the "X" button necessarily
    if ( AllPOIsOn == 0)
    {
        change_button_text( "POIs ON", "POIs OFF");
        AllPOIsOn = 1;
    }
    
    else  if ( AllPOIsOn == 1)
    {
        change_button_text("POIs OFF", "POIs ON" );
        AllPOIsOn = 0;
    }
    
    
    //Draw the screen again but with the POIs represented as red dots
    drawscreen();
}



//"Features ON" button
//Shows all the POIs on the map as red dots
void All_Features_On ( void (*drawscreen_ptr) (void)  )
{
    
    //This if else statement is used to only use one button to
    //turn the coloured Features on and off
    //No need to interact with the "X" button necessarily
    if ( AllFeaturesOn == 0)
    {
        change_button_text( "Features ON", "Features OFF");
        AllFeaturesOn = 1;
    }
    
    else  if ( AllFeaturesOn == 1)
    {
        change_button_text("Features OFF", "Features ON" );
        AllFeaturesOn = 0;
    }
    
    
    
    //Draw the screen again but with the POIs represented as red dots
    drawscreen();
}



void Add_Intersection ( void (*drawscreen_ptr) (void)  )
{
    Intersections_Saved.push_back( Intersection_Name_Entered );
}



// "Route Int-Int" Button
//Click 2 points or type in 2 intersection names. Click this button to get the shortest route
//This function uses the last two entries in vector<string> Intersections_Saved
void Route_2_Intersection ( void (*drawscreen_ptr) (void)  )
{
    //Only execute this function if we have 2 or more names in our vector Intersections_Saved
    //Or else we get a seg fault!
    
    if ( Intersections_Saved.size() >= 2 )
    {
    
    //Use iterators to get the last two elements in the vector Intersections_Saved
    //Start from the end of the vector
    vector<string>::iterator iter = (Intersections_Saved.end() - 1);
    
    //The last intersection string will be our final destination ID
    //The second last intersection string will be our intial intersection
    
    //*iter gives us the last entry in the vector
     Final_ID = find_intersection_id_from_name( *iter );
    
    //Go to the second last entry of the vector
    iter--;
    
     Initial_ID = find_intersection_id_from_name( *iter );

    
    //This Function accepts intersection IDs not strings
    The_Path_For_Drawing = find_path_between_intersections( Initial_ID, Final_ID );
        
    
    //Draw the screen again but with the path shown in blue
    drawscreen();
    
    }
    
}

//Find the route between only the LAST two points clicked
//Find the closest Interesction ID to the points clicked and find the route between those
//intersections
//An Intersection is considered to be close to a point clicked if it is < 150m away
void Route_2_Points ( void (*drawscreen_ptr) (void)  )
{
    
    //Only execute this function if we have 2 or more clicked points in our vector
    //Or else we get a seg fault!
    if ( Click_Coordinates.size() >= 2 )
    {
        
        int Last_Position = Click_Coordinates.size() - 1;
        int Second_Last_Position = (Last_Position - 1);
        
        //int count = 0;
        
    //For now, we don't have a KD Tree to find the closest intersection ID
    //so we loop over all the intersections (~100,000)
    for (int i = 0; i < getNumberOfIntersections() ; i++)
    {
        
        double Dist_2_First_Pt = find_distance_between_two_points( getIntersectionPosition(i), Click_Coordinates[Second_Last_Position]  );
        double Dist_2_Last_Pt = find_distance_between_two_points( getIntersectionPosition(i),Click_Coordinates[Last_Position]  );
        
        
        //An Intersection is considered to be close to a point clicked if it is < 150m away
        if ( Dist_2_First_Pt < 100 )
        {
            Initial_ID = i;
        }
        
        if (Dist_2_Last_Pt < 100 )
        {
            Final_ID = i;
        }
        
        
    }
    
    //This Function accepts intersection IDs not strings
    The_Path_For_Drawing = find_path_between_intersections( Initial_ID, Final_ID );

    //Draw the screen again but with the path shown in blue
    drawscreen();
        
    }
    
}

//Find the path from an intersection to an entered POI
//The intial intersection is the last element in vector<string> Intersections_Saved
//The POI is the name stored in POI_Entered
void Route_2_POI ( void (*drawscreen_ptr) (void)  )
{
    //Only execute this function if we have 1 or more names in our vector Intersections_Saved
    //Since the last element in this vector is our intial starting point, if it is empty
    //we get a seg fault!
    if ( Intersections_Saved.size() >= 1 )
    {
        
        //Use iterators to get the last element in the vector Intersections_Saved
        //Start from the end of the vector
        vector<string>::iterator iter = (Intersections_Saved.end() - 1);
        
        //The last element in the vector will give us our intial intersection ID
        //*iter gives us the last entry in the vector
        Initial_ID = find_intersection_id_from_name( *iter );
        
        
        //This Function accepts intersection IDs not strings
        The_Path_For_Drawing = find_path_to_point_of_interest( Initial_ID, POI_Entered );
        
        //Draw the screen again but with the path shown in blue
        drawscreen();
        
    }
}

//Print directions to the terminal
//Loop over the vector containing the street segments of the path found
//First mention the intial Intersection name
//Turn on (Street Segment name) etc ...
//Lastly mention the final Intersection name
void Print_Directions ( void (*drawscreen_ptr) (void)  )
{
    
    //Only execute this function if your vector of the path is not empty
    //Otherwise we get a sef fault
    if ( The_Path_For_Drawing.size() >= 1 )
    {
    
    
    int Dist_2_Diff_Street = 0;
    
    //The names of the intial and final Intersection
    string Initial_Intersection = getIntersectionName( Initial_ID );
    string Final_Intersection = getIntersectionName( Final_ID );

    cout << "----------------DIRECTIONS ----------------" << endl;
    cout << "Starting From " << Initial_Intersection << endl;
    
    //Our intial road to leave from
    cout << "Start on " << getStreetName( getStreetSegmentStreetID(The_Path_For_Drawing[0]) )<< endl;
    
    //Go over the vector of street segments
    for (int i = 0; i < (The_Path_For_Drawing.size() -1); i++)
    {
        //If two consecutive street segments have the same street name, then stop printing the
        //street names multiple # of times (Waste of text)
        if ( getStreetSegmentStreetID(The_Path_For_Drawing[i]) != getStreetSegmentStreetID(The_Path_For_Drawing[i+1]) )
        {
            Dist_2_Diff_Street = Dist_2_Diff_Street + find_street_segment_length(The_Path_For_Drawing[i]);
            
            cout << "Stay on " << getStreetName( getStreetSegmentStreetID(The_Path_For_Drawing[i]) ) << " for " << Dist_2_Diff_Street << " metres." << endl;
            
            cout << "Turn on " << getStreetName( getStreetSegmentStreetID(The_Path_For_Drawing[i+1]) ) << endl;
            
            if (i == (The_Path_For_Drawing.size() -2) )
            {
                Dist_2_Diff_Street = find_street_segment_length(The_Path_For_Drawing[i+1]);
                
                cout << "Stay on " << getStreetName( getStreetSegmentStreetID(The_Path_For_Drawing[i+1]) ) << " for " << Dist_2_Diff_Street << " metres." << endl;
            }
            
            //Set distance to next different street name as zero
            Dist_2_Diff_Street = 0;
        }
        
        //The 2 consective street segments belong to the same street
        //keep on accumulating the distance until the next DIFFERENT street name
        //to show the user (ie. Stay on Bay Street For "50 metres")
        else
        {
            Dist_2_Diff_Street = Dist_2_Diff_Street + find_street_segment_length(The_Path_For_Drawing[i]);
            
            if (i == (The_Path_For_Drawing.size() -2) )
            {
                Dist_2_Diff_Street = Dist_2_Diff_Street + find_street_segment_length(The_Path_For_Drawing[i+1]);
                
                cout << "Stay on " << getStreetName( getStreetSegmentStreetID(The_Path_For_Drawing[i]) ) << " for " << Dist_2_Diff_Street << " metres." << endl;
            }
        }
    }
    
    
    
    cout << "Reached Destination: " << Final_Intersection << endl;
    cout << "-------------------------------------------" << endl;
    
    }//End if the the vector of path is NOT empty
}


// "Courier" button
//Click on a set of points. Click this button to find the quickest route through all these points. Used by courier companies
void Courier ( void (*drawscreen_ptr) (void)  )
{
    
}



//"POI range" button
//Click a point or type in an intersection name. Enter a radius. Click this button and you will get all the POIs in that range of the chosen point or intersection
void POI_Range ( void (*drawscreen_ptr) (void)  )
{
    //User clicked on a point
    PointClicked = 1;
    
    drawscreen();
    
}



/****************************************Helper Functions*****************************************/


//Helper function to draw the natural features
void Draw_Features()

{

for (unordered_map<int,Feature>::iterator iter = Hash_Feature_Info.begin(); iter != Hash_Feature_Info.end() ; iter++)
{
    //iter->second returns the value in the hash table
    //which is a Feature object
    
    
    //Parse one feature at a time
    //Use a temp variable to store the feature
    Feature temp = (iter->second);
    
    //First Check if it is a closed feature or Open feature
    //For fillpoly(), note:
    //No need to create a t_point array just to pass it to fillpoly()
    //Waste of time
    //Since the vector is already a <t_point>, just create a t_point* and point it to the
    //address of the first element of the vector<t_point>
    if (temp.closed)
        
    {
        //"waterway" is an attribute key
        // Closed: "stream"  "riverbank"
        // Open: "river"  "riverbank"  "stream"  "weir"  "drain"
        if ( temp.Attribute_Key == "waterway")
            
        {
             if ( temp.Attribute_Value == "stream")
            {
                setcolor(Google_Blue);
                
                t_point* coordinates = &( temp.XYs_of_Feature[0] );
                fillpoly( coordinates , temp.Num_Pts_Feature);
            }
            
        }
        
        
        //"leisure" is an attribute key
        // Open & Closed: "park" or "nature_reserve"
        else if ( temp.Attribute_Key == "leisure" || temp.Attribute_Key == "landuse")
            
        {
            //All attribute values are shown as Green
            setcolor(Google_Green);
            t_point* coordinates = &( temp.XYs_of_Feature[0] );
            fillpoly( coordinates , temp.Num_Pts_Feature);
        }
        
        //"water" is an attribute key
        // Closed: "pond" "lagoon" "reservoir" "lake"
        else if ( temp.Attribute_Key == "water")
            
        {
            //All attribute values are shown as Green
            setcolor(Google_Blue);
            t_point* coordinates = &( temp.XYs_of_Feature[0] );
            fillpoly( coordinates , temp.Num_Pts_Feature);
        }
        
    
    }
    
    
    /*********************************OPEN****************************************/
    
    //It is an Open feature
    else
        
    {
        //"waterway" is an attribute key
        // Closed: "stream"  "riverbank"
        // Open: "river"  "riverbank"  "stream"  "weir"  "drain"
        if ( temp.Attribute_Key == "waterway" || temp.Attribute_Key == "natural")
            
        {
            if ( temp.Attribute_Value == "river" || temp.Attribute_Value == "stream" || temp.Attribute_Value == "water")
            {
                setlinewidth (3);
                setcolor(Google_Blue);
                
                for (int i = 0; i < ( (temp.XYs_of_Feature).size() - 1 ) ; i++)
                {
                    drawline( temp.XYs_of_Feature[i] , temp.XYs_of_Feature[i+1]);
                }
            }
        
        }

    }
    
    
}


}































void Draw_Streets(float UI_Area)
{
    //Check what zoom level we are at
    //Setting the color once speeds up the drawing by alot!
    
    //Past the point StreetView_LVL3, don't draw the streets < 5000m
    //In order to do this, check if the street segment being drawn is part of a street that
    //is less than or greater than 5000m
    //The street length is stored in Hash_Street_Info
    if ( UI_Area > StreetView_LVL3 )
    {
        
        //Draw the long streets first
        setcolor (WHITE);
        setlinewidth(2);
        for (int i = 0; i < StreetSegIDs_Zoom3.size(); i++)
        {
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( StreetSegIDs_Zoom3[i] );
            
            //(x,y) coordinates of all the points of the street segment
            vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
            for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
            {
                //The parameters are two t_point types
                drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
        }
        
        //Draw the highways
        setcolor (CORAL);
        setlinewidth(5);
        for (int i = 0; i < StreetSegIDs_Highway.size(); i++)
        {
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( StreetSegIDs_Highway[i] );
            
            //(x,y) coordinates of all the points of the street segment
            vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
            for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
            {
                //The parameters are two t_point types
                drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
        }
        
    }// end if we are farther than zoom level 3
    
    
    
    
    //Past the point StreetView_LVL2, draw minor streets in light grey
    //Minor streets are those < 5000m
    //The street length is stored in Hash_Street_Info
    else if ( UI_Area > StreetView_LVL2  )
    {
        
        //Draw the minor street segments belonging to streets < 5000m
        setcolor (LIGHTGREY);
        setlinewidth(1);
        for (int i = 0; i < StreetSegIDs_Zoom2_Grey.size(); i++)
        {
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( StreetSegIDs_Zoom2_Grey[i] );
            
            //(x,y) coordinates of all the points of the street segment
            vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
            for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
            {
                //The parameters are two t_point types
                drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
        }

        
        //Draw the long streets after it
        setcolor (WHITE);
        setlinewidth(3);
        for (int i = 0; i < StreetSegIDs_Zoom3.size(); i++)
        {
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( StreetSegIDs_Zoom3[i] );
            
            //(x,y) coordinates of all the points of the street segment
            vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
            for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
            {
                //The parameters are two t_point types
                drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
        }
        
        //Draw the highways over on top
        setcolor (CORAL);
        setlinewidth(5);
        for (int i = 0; i < StreetSegIDs_Highway.size(); i++)
        {
            unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find( StreetSegIDs_Highway[i] );
            
            //(x,y) coordinates of all the points of the street segment
            vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
            for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
            {
                //The parameters are two t_point types
                drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
        }
        
    }//End if we are farther than zoom level 2
    
    
    
    //At this level also display the street names
    else if ( UI_Area < StreetView_LVL1  )
    {
        
        setcolor (WHITE);
        
        for (unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.begin(); iter != Hash_StreetSegment_Info.end() ; iter++)
        {
        
        //Get the Street name this Street Segment belongs to
        string StreetSeg_Name = (iter->second).Name_of_StreetSeg;
        
        //(x,y) coordinates of all the points of the street segment
        vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
        unordered_map<string,Street>::iterator iter2 = Hash_Street_Info.find( StreetSeg_Name );

            
            //Make sure, it is not unknown
            //Because all unknown streets are clumped into one category : (unknown)
            if ( (iter2->second).length > 2000 && StreetSeg_Name != "(unknown)" )
                //Draw it at this zoom level
            {
                
                for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
                {
                    //Since there is no "highway" attribute key on the map
                    //Figure out highways using the street segment speed limit
                    //Highways are usually with speed limit > 80Km/hr
                    //Draw highways (speed limit > 80) with orange (CORAL)
                    //Draw it with a thicker line as well
                    if ( (iter->second).StreetSegment_SpeedLimit > 80 )
                    {
                        setcolor (CORAL);
                        setlinewidth (4);
                        drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
                        setcolor (WHITE);
                    }
                    
                    
                    //else the speed limit was < 80
                    else
                    {
                        setlinewidth (4);
                        //The parameters are two t_point types
                        drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
                    }
                }
                
            }
            
            else
            {
                setlinewidth (1);
                
                for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
                    
                    drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
            }
            
            
            
            
            /************************Display Street Names************************************/
            
            
            //If we are zoomed way in, Display the street names for the user
            //User should be very zoomed in
            //Write the names in black
            //Before street names appeared after 5.57851e-08 zoom in level, if it was good change it
            if ( UI_Area < StreetName_LVL  && (iter->second).Name_of_StreetSeg != "(unknown)")
            {
                //Color of the street names
                setcolor (BLACK);
                
                //Angle of text depends on the intial and final (x,y) coordinates of the street segment
                //The relation between the line passing through those points and the horizantal line
                //Get the first and last (x,y) point of the street segment
                //Angle is in degrees
                float Angle_of_Text;
                
                vector<t_point>::iterator iter1 = XandYCoordinates.begin();
                
                //Coordinates of the first point of the street segment
                t_point First_StreetSeg_Point(*iter1);
                
                
                
                t_point Last_StreetSeg_Point;
                
                
                //Check if the StreetSegment is curved or straight
                
                //Then it has curve points
                if ( XandYCoordinates.size() > 2 )
                {
                    //Find the mid-point coordinate of this curved segment
                    //If it is an odd sized vector, it will get casted
                    int Midpoint_Coordinate = XandYCoordinates.size() / 2;
                    
                    Last_StreetSeg_Point.x = XandYCoordinates[Midpoint_Coordinate].x;
                    Last_StreetSeg_Point.y = XandYCoordinates[Midpoint_Coordinate].y;
                    
                }
                
                //else it is a straight segment
                else
                    
                {
                    iter1 = ( XandYCoordinates.end() - 1 );
                    
                    Last_StreetSeg_Point.x = (*iter1).x;
                    Last_StreetSeg_Point.y = (*iter1).y;
                    
                    //Coordinates of the last point of the street segment
                    // t_point Last_StreetSeg_Point(*iter1);
                }
                
                
                //Cover corner cases for the Angle_of_Text
                //If the points are right above each other (ie. same x coordinate: Then the angle of the text should be -90)
                //Ie. Vertical angle
                if ( First_StreetSeg_Point.x == Last_StreetSeg_Point.x )
                    
                    Angle_of_Text = -90.0;
                
                else
                    
                    //* by (180/PI) to convert it to degrees
                    Angle_of_Text = atan( (Last_StreetSeg_Point.y - First_StreetSeg_Point.y )/ (Last_StreetSeg_Point.x - First_StreetSeg_Point.x ) )* (180/pi);
                
                
                
                //Set the font and the angle of the text to be written
                settextattrs(11, Angle_of_Text);
                
                //in order to not clutter the screen with texts
                //Make a bounding box for the text
                //No need to add level of detail anymore
                float boundsX = abs( (Last_StreetSeg_Point.x - First_StreetSeg_Point.x ) );
                float boundsY = abs ( (Last_StreetSeg_Point.y - First_StreetSeg_Point.y ) );
                
                //The (x,y) center coordinates for the text to be written
                float TextCenter_X = (Last_StreetSeg_Point.x + First_StreetSeg_Point.x ) /2;
                float TextCenter_Y = (Last_StreetSeg_Point.y + First_StreetSeg_Point.y ) /2;
                
                drawtext(TextCenter_X, TextCenter_Y, (iter->second).Name_of_StreetSeg , boundsX , boundsY);
                
                setcolor (WHITE);
            }

            
            
            
            
        
        }//End for loop through Hash_StreetSegments_Info
    }//End if we are closer than zoom level 1
    
    
    
    //We did not meet any of the above constraints
    else
    {
        setlinewidth(2);
        setcolor (WHITE);
        
        
        for (unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.begin(); iter != Hash_StreetSegment_Info.end() ; iter++)
        {
        
        //(x,y) coordinates of all the points of the street segment
        vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
            
        //Obtained all the (x,y) coordinates of a single street segment
        //Now go through all those (x,y) coordinates in the vector and connect them to draw the street segment
        //Use drawline() in graphics.h
        for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
        {
         
            //The parameters are two t_point types
            drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );

        }
        
        }//End for loop through all street segments
        
        
    }//End didnt meet any constraints
    
    
    
    
    
    
    

    


    //Drawing the path between two selected intersections in Blue
    setlinewidth (5);
    setcolor (Google_Path_Blue);
    
    for (int i = 0; i < The_Path_For_Drawing.size(); i++)
    {
        //Obtain the Street Segment ID which is part of the total path
        int StreetSegID = The_Path_For_Drawing[i];
        
        //Find the information of this StreetSegment in Hash_StreetSegment_Info
        unordered_map<int,StreetSegment>::iterator iter = Hash_StreetSegment_Info.find(StreetSegID);
        
        vector<t_point> XandYCoordinates = (iter->second).XY_Coordinates_StreetSegmentPoints;
        
        for (int i = 0; i < (XandYCoordinates.size() - 1) ; i++)
        {
            drawline( XandYCoordinates[i], XandYCoordinates[i+1]  );
        }
        
    }

    
    
    
    //Draw the intial and final destination points
    //Make sure the vector is not empty
    //Inital ID and Final ID are global vectors
    if ( The_Path_For_Drawing.size() > 2)
    {
        LatLon Intersection_Location = getIntersectionPosition( Initial_ID );
        
        //Convert the LatLon to (x,y) coordinates using function in m1.h
        t_point XY = Convert_LatLon_to_XY( Intersection_Location );
        
        setcolor (RED);
        
        fillarc(XY, 0.00001, 0, 360);
        
        Intersection_Location = getIntersectionPosition( Final_ID );
        
        XY = Convert_LatLon_to_XY( Intersection_Location );
        
        setcolor (LIMEGREEN);
        
        fillarc(XY, 0.00001, 0, 360);
    }
    

}




//Given the stem 'stem_text' perform auto completion.
//It returns an array of strings that are potential completions
//
//Note:
// 'start' and 'end' denote the location of 'stem_text' in the global
// 'rl_line_buffer' variable which contains the users current input line.
// If you need more context information to determine how to complete 'stem_text'
// you could look at 'rl_line_buffer'.
char** command_completion(const char* stem_text, int start, int end) {
    char ** matches = NULL;
    
    if(start != 0) {
        //Only generate completions if stem_text'
        //is not the first thing in the buffer
        matches = rl_completion_matches(stem_text, intersection_name_generator);
    }
    
    return matches;
}

//Called repeatedly for a given 'stem_text'. Each time it returns a potential
//match.  When there are no more matches it returns NULL.
//
//The 'state' variable is zero the first time it is called with a given
//'stem_text', and positive afterwards.
char* intersection_name_generator(const char* stem_text, int state) {
    //Static here means a variable's value persists across function invocations
    static int count;
    
    if(state == 0) {
        //We initialize the count the first time we are called
        //with this stem_text
        count = -1;
    }
    
    int text_len = strlen(stem_text);
    
    //Search through Intersection_Names_AutoCompletion until we find a match
    while(count < (int) Intersection_Names_AutoCompletion.size()-1) {
        count++;
        if(strncmp(Intersection_Names_AutoCompletion[count], stem_text, text_len) == 0) {
            //Must return a duplicate, Readline will handle
            //freeing this string itself.
            return strdup(Intersection_Names_AutoCompletion[count]);
        }
    }
    
    //No more matches
    return NULL;
}













