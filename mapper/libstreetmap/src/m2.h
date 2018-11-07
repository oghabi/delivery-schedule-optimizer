#pragma once

//Include m1.h inorder to access Hash_StreetSegment_Info to draw the map
//Hash_StreetSegment_Info is a hash table which has a value of a StreetSegment Object
#include "m1.h"


//Defines different level of zooms
//As the Level # increases, the zoom out view increases
/********************************************************************************************/
#define StreetView_LVL1 5.6e-08 //At this level, draw minor streets same as normal streets
#define StreetView_LVL2 3.3e-06 //At this level draw minor streets with thin lines (Dark grey)
#define StreetView_LVL3 7.089e-05 //At this level don't draw the minor streets


//At this level
#define StreetName_LVL 5.55498e-08 //At this level, show the street names

/********************************************************************************************/

extern vector<char> User_Input;
extern vector<string> Intersections_Saved;
extern char* Buffer_UserInput;
extern vector<LatLon> Click_Coordinates;
extern bool IntersectionEntered_Found;
extern bool AllPOIsOn;
extern bool AllFeaturesOn;
extern bool PointClicked;
extern bool POI_Found;



// Draws the map whose at map_path; this should be a .bin file.
void draw_map(std::string map_path);


//The callback functions passed on to event_loop();
void act_on_key_press (char c);
void act_on_mouse_move (float x, float y);
void drawscreen();
void act_on_button_press (float x, float y, t_event_buttonPressed event);



//The Buttons I Added
void undo_button ( void (*drawscreen_ptr) (void)  );
void Add_Intersection ( void (*drawscreen_ptr) (void)  );
void Route_2_Intersection ( void (*drawscreen_ptr) (void)  );
void Route_2_Points ( void (*drawscreen_ptr) (void)  );
void Route_2_POI ( void (*drawscreen_ptr) (void)  );
void Print_Directions ( void (*drawscreen_ptr) (void)  );
void Courier ( void (*drawscreen_ptr) (void)  );
void POI_Range ( void (*drawscreen_ptr) (void)  );
void All_POIs_On ( void (*drawscreen_ptr) (void)  );
void All_Features_On ( void (*drawscreen_ptr) (void)  );




//Helper functions

//Functions to use in drawscreen()
void Draw_Features();
void Draw_Streets(float UI_Area);



//readline functons
char** command_completion(const char* stem_text, int start, int end);
char* intersection_name_generator(const char* stem_text, int state);