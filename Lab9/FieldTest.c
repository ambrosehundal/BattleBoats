// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

#include "Field.h"

// User libraries


// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****


// **** Declare any function prototypes here ****


int main() {
    BOARD_Init();



    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    /*
     * Testing for FieldInit()
     */
    Field f;
    Field k;
    Field* p;
    p = &k;
    Field g;
    Field* m;
    Field* n;
    m = &f;
    n = &g;
    FieldInit(m, FIELD_POSITION_EMPTY);
    FieldInit(p, FIELD_POSITION_LARGE_BOAT);
    FieldInit(n, FIELD_POSITION_MEDIUM_BOAT);

    //Testing for FieldAt()    
    //When calling for FieldAt(), check for array index
    FieldAt(p, 4, 6);
    FieldAt(m, 3, 5);
    FieldAt(p, 3, 4);
    FieldAt(p, 5, 9);


    //Testing for FieldSetLocation()
    //  FieldSetLocation(p
    FieldSetLocation(p, 3, 4, FIELD_POSITION_HUGE_BOAT);
    FieldSetLocation(m, 3, 5, FIELD_POSITION_SMALL_BOAT);


    //Testing for FieldAddBoat()
    FieldAddBoat(m, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    //  FieldAddBoat(m, 3, 5, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    //FieldPosition a, b, c;
    // a = f->
     printf("Small boat values check if they are placed right: %d, %d, %d\n",
        m->field[0][0], m->field[0][1], m->field[0][2]);






    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}
