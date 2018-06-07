#include "Protocol.h"
#include "Field.h"
#include "Agent.h"
#include "BOARD.h"
#include "Oled.h"
#include "OledDriver.h"
#include "FieldOled.h"


/*
 * typedef enum {
    FIELD_OLED_TURN_NONE,
    FIELD_OLED_TURN_MINE,
    FIELD_OLED_TURN_THEIRS
} FieldOledTurn;
 
 */



//Static variables
static Field myField;
static Field enemyField;
static NegotiationData myNegotiation;
static NegotiationData enemyNegotiation;
static GuessData myGuess;
static GuessData enemyGuess;
static AgentEvent event;
static FieldOledTurn turn;
//static BoatType type;

static uint8_t r;
static uint8_t c;
BoatType direction;
int success = 0;
uint8_t small = FALSE, medium = FALSE, large = FALSE, huge = FALSE;

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */

void AgentInit(void) {
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_UNKNOWN);

    while (success < 4) {
        r = rand() % 6;
        c = rand() % 10;
        direction = rand() % FIELD_BOAT_DIRECTION_WEST;

        if (small == FALSE) {
            small = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_SMALL);
            //check if boat was successfully placed
            if (small == TRUE) {
                success++;
            }
        } else if (large == FALSE) {
            large = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_LARGE);
            //check if boat was successfully placed
            if (large == TRUE) {
                success++;
            }
        } else if (medium == FALSE) {
            medium = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_MEDIUM);
            //check if boat was successfully placed
            if (medium == TRUE) {
                success++;
            }
        } else if (huge == FALSE) {
            huge = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_HUGE);
            //check if boat was successfully placed
            if (huge == TRUE) {
                success++;
            }
        }

    }






    FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_SMALL);
    FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_MEDIUM);
    FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_LARGE);
    FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_HUGE);

    FieldOledDrawScreen(&myField, &enemyField, turn);




}

int AgentRun(char in, char *outBuffer) {
    AgentState state = AGENT_STATE_GENERATE_NEG_DATA;
    int length = 0;

    switch (state) {
        case AGENT_STATE_GENERATE_NEG_DATA:
            ProtocolGenerateNegotiationData(&myNegotiation); //Generate negotiation data for my field
            length = ProtocolEncodeChaMessage(outBuffer, &myNegotiation); // Send challenge data
            state = AGENT_STATE_SEND_CHALLENGE_DATA; // Change state to SEND_DATA
            break;
        case AGENT_STATE_SEND_CHALLENGE_DATA:
            if (event == AGENT_EVENT_RECEIVED_CHA_MESSAGE) { // Once we receive CHA message
                if (ProtocolDecode(in, &enemyNegotiation, &enemyGuess) == PROTOCOL_PARSING_FAILURE) { // Record opponent data, check if parsing failure
                    state = AGENT_STATE_INVALID; // Change state to invalid if parsing failure
                } else { // If message parsing doesn't return fail
                    length = ProtocolEncodeDetMessage(outBuffer, &myNegotiation); // Send determine data
                    state = AGENT_STATE_DETERMINE_TURN_ORDER; // Change state to DETERMINE_ORDER
                }
            }
            break;
        case AGENT_STATE_DETERMINE_TURN_ORDER:
            if (event == AGENT_EVENT_RECEIVED_DET_MESSAGE) { // Once we receive DET message
                if (ProtocolValidateNegotiationData(&enemyNegotiation) == TRUE) { // Validate opponent negotiation data, check if valid
                    if (ProtocolGetTurnOrder(&myNegotiation, &enemyNegotiation) == TURN_ORDER_START) { // If you won turn ordering
                        turn = FIELD_OLED_TURN_MINE; // Set turn to your turn
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        OledUpdate();
                        state = AGENT_STATE_SEND_GUESS; // Change state to SEND_GUESS

                    } else if (ProtocolGetTurnOrder(&myNegotiation, &enemyNegotiation) == TURN_ORDER_DEFER) { // If opponent won turn ordering
                        turn = FIELD_OLED_TURN_THEIRS; // Set turn to their turn
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        OledUpdate();
                        state = AGENT_STATE_WAIT_FOR_GUESS; // Changes state to WAIT_FOR_GUESS
                    } else { // If turn ordering was a tie
                        OledClear(OLED_COLOR_BLACK);
                        OledUpdate();
                        // Set OLED to ERROR_STRING_ORDERING
                    }
                } else { // If opponent negotiation data is invalid
                    OledClear(OLED_COLOR_BLACK);
                    // Set OLED to ERROR_STRING_ORDERING
                    state = AGENT_STATE_INVALID; // Change state to INVALID
                }
            }
            break;
        case AGENT_STATE_SEND_GUESS:
            if (turn == FIELD_OLED_TURN_MINE) { // Check if your turn
                myGuess.row = rand() % 6; // Randomize row
                myGuess.col = rand() % 10; // Randomize col
                if (enemyField.field[myGuess.row][myGuess.col] == FIELD_POSITION_UNKNOWN) { // Checks for valid coordinates
                    length = ProtocolEncodeCooMessage(outBuffer, &myGuess); // Send COO message
                    state = AGENT_STATE_WAIT_FOR_HIT; // Change state to WAIT_FOR_HIT
                }
            }
            break;
        case AGENT_STATE_WAIT_FOR_HIT:
            if (ProtocolDecode(in, &enemyNegotiation, &enemyGuess) == PROTOCOL_PARSING_FAILURE) { // Record HIT message, check if parsing failure
                state = AGENT_STATE_INVALID; // Change state to INVALID
            } else { // If message parsing doesn't return fail
                if (FieldGetBoatStates(&enemyField) != 0) { // If enemy boats left
                    FieldUpdateKnowledge(&enemyField, &myGuess);
                    turn = FIELD_OLED_TURN_MINE; // Set to my turn
                    FieldOledDrawScreen(&myField, &enemyField, turn);
                    OledUpdate();
                    state = AGENT_STATE_WAIT_FOR_GUESS; // Change to WAIT_FOR_GUESS
                } else { // If no more enemy boats left
                    turn = FIELD_OLED_TURN_NONE; // Set turn to none
                    FieldOledDrawScreen(&myField, &enemyField, turn);
                    OledUpdate();
                    state = AGENT_STATE_WON; // Change state to won
                }
            }
            break;
        case AGENT_STATE_WAIT_FOR_GUESS:
            if (turn == FIELD_OLED_TURN_THEIRS) { // Check if their turn
                if (ProtocolDecode(in, &enemyNegotiation, &enemyGuess) == PROTOCOL_PARSING_FAILURE) { // Record COO message, check if parsing failure
                    state = AGENT_STATE_INVALID; // Change state to INVALID
                } else { // If message parsing doesn't return fail
                    if (FieldGetBoatStates(&myField) != 0) { // If friendly boats left
                        turn = FIELD_OLED_TURN_MINE; // Set turn to my turn
                        FieldRegisterEnemyAttack(&myField, &enemyGuess); // Register hit
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        OledUpdate();
                        length = ProtocolEncodeHitMessage(outBuffer, &myGuess); // Send HIT message
                        state = AGENT_STATE_SEND_GUESS; // Change state to SEND_GUESS
                    } else { // If no more friendly boats left
                        turn = FIELD_OLED_TURN_NONE; // Set turn to none
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        OledUpdate();
                        length = ProtocolEncodeHitMessage(outBuffer, &myGuess); // Send HIT message
                        state = AGENT_STATE_LOST; // Change state to LOST
                    }
                }
            }
            break;
        case AGENT_STATE_INVALID:
            length = 0;
            break;
        case AGENT_STATE_LOST:
            length = 0;
            break;
        case AGENT_STATE_WON:
            length = 0;
            break;
    }
    return length;
}

uint8_t AgentGetStatus(void) {
    return FieldGetBoatStates(&myField);
}

/**
 * This function returns the same data as `AgentCheckState()`, but for the enemy agent.
 * @return A bitfield indicating the sunk/unsunk status of each ship under the enemy agent's
 *         control.
 *
 * @see Field.h:FieldGetBoatStates()
 * @see Field.h:BoatStatus
 */
uint8_t AgentGetEnemyStatus(void) {
    return FieldGetBoatStates(&enemyField);
}


