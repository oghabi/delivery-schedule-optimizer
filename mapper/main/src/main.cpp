#include "StreetsDatabaseAPI.h"
#include "m2.h"
#include "graphics.h"
#include <iostream>
using namespace std;



int main() {

    
    draw_map("/cad2/ece297s/public/maps/toronto.bin");
    close_map();
    
    
//
//    //Ask the user which maps he wants to be shown
//    
//    //The Current_Map being drawn
//    //This is the buffer of line entered by the user
//    char* Current_Map;
//    
//    //Choose one of the the following maps
//    cout << "Welcome! Please choose one of the following maps: " << endl;
//    cout << "Cairo    Hamilton    Moscow    New York    Toronto" << endl;
//    cout << "To exit the program. Type Exit" << endl;
//    
//    
//    
//    
//    //Note: with readline you can use the arrow keys like a cursor
//    //and you can also use the backspace key to delete any mistakes you make
//    while ( ( Current_Map = readline("") )  != NULL ) //Ask the user to enter something
//    {
//     
//        //Exit the program loop is user entered "Exit"
//        //readline is written in C, not C++
//        if ( strcmp( Current_Map , "Exit") == 0 ) break;
//        
//        
//            if ( strcmp( Current_Map , "Toronto") == 0 )
//            {
//                //Draw the map of Toronto
//                draw_map("/cad2/ece297s/public/maps/toronto.bin");
//                close_map();
//                goto NextMap_or_Exit;
//            }
//        
//            else if ( strcmp( Current_Map , "New York") == 0)
//            {
//                draw_map("/cad2/ece297s/public/maps/newyork.bin");
//                close_map();
//                goto NextMap_or_Exit;
//            }
//                
//            else if ( strcmp( Current_Map , "Hamilton") == 0 )
//            {
//                draw_map("/cad2/ece297s/public/maps/hamilton_canada.bin");
//                close_map();
//                goto NextMap_or_Exit;
//            }
//                
//            else if ( strcmp( Current_Map , "Cairo") == 0 )
//            {
//                draw_map("/cad2/ece297s/public/maps/cairo_egypt.bin");
//                close_map();
//                goto NextMap_or_Exit;
//            }
//                
//            else if ( strcmp( Current_Map , "Moscow") == 0 )
//            {
//                draw_map("/cad2/ece297s/public/maps/moscow.bin");
//                close_map();
//                goto NextMap_or_Exit;
//            }
//                
//            //The name entered was not recognized
//            
//            else
//            {
//                cout << "Sorry! That map is not available. Please choose one of the following: " << endl;
//                cout << "Cairo    Hamilton    Moscow    New York    Toronto" << endl;
//                cout << "To exit the program. Type Exit" << endl;
//                goto Next;
//            }
//            
//        
//        
//    //Either draw another map or exit
//    NextMap_or_Exit:
//
//                cout << "View our other available maps!: " << endl;
//                cout << "Cairo    Hamilton    Moscow    New York    Toronto" << endl;
//                cout << "To exit the program. Type Exit" << endl;
//    
//        Next:
//        
//                //readline generates a new(dynamic) buffer every time,
//                //so we need to free the current one after we've finished
//                free(Current_Map);
//                //Mark it null to show we freed it
//                Current_Map = NULL;
//        
//
//    }
//    
//    
//    
//    //If the buffer wasn't freed in the main loop when we suddenly exited, we need to free it now
//    // Note: if buf is NULL free does nothing
//    free(Current_Map);
    
    
    return 0;
}

















