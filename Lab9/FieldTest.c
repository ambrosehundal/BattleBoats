// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

#include "Field.h"
#include "Protocol.h"

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
    int ftest = 0;

    Field f;
    Field ka;
    Field* p;
    p = &ka;
    Field g;
    Field* m;
    Field* n;
    m = &f;
    n = &g;
    FieldInit(m, FIELD_POSITION_EMPTY);
    FieldInit(p, FIELD_POSITION_LARGE_BOAT);
    FieldInit(n, FIELD_POSITION_MEDIUM_BOAT);

    printf("Initialized field positions for the field are: %d, %d, %d, %d, %d, "
            "%d\n",
            m->field[0][0], m->field[1][1], m->field[2][2], m->field[3][3],
            m->field[4][7], m->field[5][7]);

    printf("Initialized field positions for the field are: %d, %d, %d, %d, %d, "
            "%d\n",
            p->field[0][0], p->field[1][1], p->field[2][2], p->field[3][3],
            p->field[4][7], p->field[5][7]);

    printf("Initialized field positions for the field are: %d, %d, %d, %d, %d, "
            "%d\n",
            n->field[0][0], n->field[1][1], n->field[2][2], n->field[3][3],
            n->field[4][7], n->field[5][7]);


    if (m->field[4][6] == 0) {
        ftest++;
        printf("Test passed!\n");
    }


    if (n->field[4][6] == 2) {
        ftest++;
        printf("Test passed!\n");
    }

    if (ftest == 2) {
        printf("FieldInit() tests passed: %d/2\n", ftest);
    }



    //Testing for FieldAt()    
    //When calling for FieldAt(), check for array index
    int fa = 0;
    FieldAt(p, 4, 6);
    if (p->field[4][6] == 3) {
        printf("Test passed!\n");
        fa++;
    }
    FieldAt(m, 3, 5);
    if (m->field[4][6] == 0) {
        printf("Test passed!\n");
        fa++;
    }

    if (fa == 2) {
        printf("FieldAt() tests passed: %d/2\n", fa);
    }






    //Testing for FieldSetLocation()
    int fp = 0;
    FieldSetLocation(p, 3, 4, FIELD_POSITION_SMALL_BOAT);
    if (FieldAt(p, 3, 4) == 1) {
        printf("Test passed!\n");
        fp++;
    }
    FieldSetLocation(m, 3, 5, FIELD_POSITION_HUGE_BOAT);
    if (FieldAt(m, 3, 5) == 4) {
        printf("Test passed!\n");
        fp++;
    }

    if (fp == 2) {
        printf("FieldSetLocation() tests passed: %d/2\n", fp);
    }




    /*
     * 
     * /Testing for FieldAddBoat()
     */
    int fab = 0;
    //Cases where Boats are successfully added
    Field testField;
    Field* t;
    t = &testField;
    FieldInit(&testField, FIELD_POSITION_EMPTY);
    FieldAddBoat(&testField, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_HUGE);

    FieldAddBoat(&testField, 4, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_LARGE);

    FieldAddBoat(&testField, 1, 0, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_SMALL);
    FieldAddBoat(&testField, 1, 5, FIELD_BOAT_DIRECTION_SOUTH, FIELD_BOAT_MEDIUM);

    //Cases where boats fail to add
    FieldAddBoat(&testField, 0, 3, FIELD_BOAT_DIRECTION_NORTH, FIELD_BOAT_MEDIUM);
    FieldAddBoat(&testField, 1, 0, FIELD_BOAT_DIRECTION_NORTH, FIELD_BOAT_MEDIUM);

    if (FieldAt(t, 0, 4) == 4) {
        printf("Test passed!\n");
        fab++;
    } else {
        printf("Test not passed!\n");
    }
    if (FieldAt(t, 4, 2) == 3) {
        fab++;
        printf("Test passed!\n");
    }

    if (fab == 2) {
        printf("FieldAddBoat() tests passed: %d/2\n", fab);
    }



    /*
     * Testing for FieldRegister Enemy attack
     */
    int ea = 0;
    Field k;
    Field* r;
    r = &k;
    FieldInit(&k, FIELD_POSITION_EMPTY);
    FieldAddBoat(r, 0, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_SMALL);
    GuessData guess;
    guess.col = 0;
    guess.row = 0;

    GuessData* g1;
    g1 = &guess;
    FieldRegisterEnemyAttack(&k, g1);
    guess.col = 1;
    guess.row = 0;
    FieldRegisterEnemyAttack(&k, g1);
    if (r->smallBoatLives == 1) {
        printf("Boat hit! Test passed!\n");
        ea++;
    }

    //  guess.col = 2;
    //  guess.row = 0;
    FieldRegisterEnemyAttack(&k, g1);
    if (r->smallBoatLives == 0) {
        printf("Boat sunk! Test passed!\n");
        ea++;
    }


    if (ea == 2) {
        printf("FieldRegisterEnemyAttack() tests passed: %d/2\n", ea);
    }

    /*
     * Testing for FieldUpdateKnowledge
     */
    int uk = 0;
    guess.col = 2;
    guess.row = 0;
    guess.hit = HIT_SUNK_SMALL_BOAT;

    FieldUpdateKnowledge(&k, g1);
    printf("Smallboat lives = %d\n", r->smallBoatLives);
    if (r->smallBoatLives == 0) {
        printf("Boat sunk! Test passed!\n");
        uk++;
    }
    FieldAddBoat(r, 1, 0, FIELD_BOAT_DIRECTION_EAST, FIELD_BOAT_MEDIUM);
    guess.col = 0;
    guess.row = 1;
    FieldRegisterEnemyAttack(&k, g1);
    guess.col = 1;
    guess.row = 1;
    FieldRegisterEnemyAttack(&k, g1);
    guess.col = 2;
    guess.row = 1;
    FieldRegisterEnemyAttack(&k, g1);
    guess.col = 3;
    guess.row = 1;
    FieldRegisterEnemyAttack(&k, g1);
    FieldUpdateKnowledge(&k, g1);
    guess.hit = HIT_SUNK_MEDIUM_BOAT;
    printf("Medium boat lives = %d\n", r->mediumBoatLives);

    if (r->mediumBoatLives == 0) {
        uk++;
    }

    if (uk == 2) {
        printf("FieldUpdateKnowledge() tests passed: %d/2\n", uk);
    }

    /*
     * Testing for BoatStates
     */
    Field eg;
    Field* e;
    e = &eg;
    e->smallBoatLives = 0;
    e->mediumBoatLives = 0;
    e->largeBoatLives = 0;
    e->hugeBoatLives = 1;
    
   uint8_t boat = FieldGetBoatStates(e);
   //PrintBoat(boat);

   //PrintBoat
    



    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}
