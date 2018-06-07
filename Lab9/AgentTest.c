// **** Include libraries here ****
// Standard libraries

//CMPE13 Support Library
#include "BOARD.h"

// Microchip libraries
#include <xc.h>
#include <plib.h>

// User libraries
#include "Agent.h"
#include "Protocol.h"
#include "Field.h"
// **** Set any macros or preprocessor directives here ****

// **** Declare any data types here ****

// **** Define any module-level, global, or external variables here ****


// **** Declare any function prototypes here ****

int main() {
    BOARD_Init();
    // Field myField;
    // FieldInit(&myField, FIELD_POSITION_EMPTY);


    /******************************************************************************
     * Your code goes in between this comment and the following one with asterisks.
     *****************************************************************************/
    //Testing for AgentInit()
    AgentInit();
    
    
    

    //Testing for AgentGetStatus
    int a = 0, b = 0;
    uint8_t agentlives = AgentGetStatus();
    if (agentlives == 0) {
        a++;
        printf("Test passed!\n");
    }

    if (a == 1) {
        printf("AgentGetStatus() tests PASSSED %d/1\n", a);
    }
    
    
    //Testing for AgentGetEnemyStatus()
    
    uint8_t enemylives = AgentGetEnemyStatus();
    if (enemylives == 0) {
        b++;
        printf("Test passed!\n");
    }

    if (b == 1) {
        printf("AgentEnemyGetStatus() tests PASSSED %d/1\n", b);
    }






    /******************************************************************************
     * Your code goes in between this comment and the preceeding one with asterisks
     *****************************************************************************/

    while (1);
}
