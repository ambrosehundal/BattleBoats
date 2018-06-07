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
        }
        else if (large == FALSE) {
            large = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_LARGE);
            //check if boat was successfully placed
            if (large == TRUE) {
                success++;
            }
        }
        else if (medium == FALSE) {
            medium = FieldAddBoat(&myField, r, c, direction, FIELD_BOAT_MEDIUM);
            //check if boat was successfully placed
            if (medium == TRUE) {
                success++;
            }
        }
        else if (huge == FALSE) {
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
            ProtocolGenerateNegotiationData(&myNegotiation); //Generate NegotiationData for my field
            length = ProtocolEncodeChaMessage(outBuffer, &myNegotiation); // Encodes challenge message and output it to buffer
            state = AGENT_STATE_SEND_CHALLENGE_DATA;
            break;
        case AGENT_STATE_SEND_CHALLENGE_DATA:
            if (event == AGENT_EVENT_RECEIVED_CHA_MESSAGE) {
                //  ProtocolDecode(in, enemyNegotiation);
                length = ProtocolEncodeDetMessage(outBuffer, &myNegotiation);
                state = AGENT_STATE_DETERMINE_TURN_ORDER;
            }
            break;
        case AGENT_STATE_DETERMINE_TURN_ORDER:
            if (event == AGENT_EVENT_RECEIVED_DET_MESSAGE) {
                if (ProtocolValidateNegotiationData(&enemyNegotiation) == TRUE) { // If opponent negotiation data is valid
                    if (ProtocolGetTurnOrder(&myNegotiation, &enemyNegotiation) == TURN_ORDER_START) { // If you won turn ordering
                        turn = FIELD_OLED_TURN_MINE;
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        state = AGENT_STATE_SEND_GUESS;

                    } else if (ProtocolGetTurnOrder(&myNegotiation, &enemyNegotiation) == TURN_ORDER_DEFER) { // If you didn't win turn ordering
                        turn = FIELD_OLED_TURN_THEIRS;
                        FieldOledDrawScreen(&myField, &enemyField, turn);
                        state = AGENT_STATE_WAIT_FOR_GUESS;
                    } else { // If turn ordering was a tie
                        OledClear(OLED_COLOR_BLACK);
                        // Set OLED to ERROR_STRING_ORDERING
                    }
                } else { // If opponent negotiation data is invalid
                    OledClear(OLED_COLOR_BLACK);
                    // Set OLED to ERROR_STRING_ORDERING
                    state = AGENT_STATE_INVALID;
                }
            }
            break;
        case AGENT_STATE_SEND_GUESS:
            if (turn == FIELD_OLED_TURN_MINE) {
                myGuess.row = rand() % 6;
                myGuess.col = rand() % 10;
                length = ProtocolEncodeCooMessage(outBuffer, &myGuess);
                state = AGENT_STATE_WAIT_FOR_HIT;
            }
            break;
        case AGENT_STATE_WAIT_FOR_HIT:
            ProtocolDecode(in, &enemyNegotiation, &enemyGuess); // Not sure whether to store in my data or enemy data

            if (FieldGetBoatStates(&enemyField) != 0) {
                FieldUpdateKnowledge(&enemyField, &myGuess);
                turn = FIELD_OLED_TURN_MINE;
                FieldOledDrawScreen(&myField, &enemyField, turn);
                state = AGENT_STATE_WAIT_FOR_GUESS;
            } else {
                turn = FIELD_OLED_TURN_MINE;
                FieldOledDrawScreen(&myField, &enemyField, turn);
                state = AGENT_STATE_WON;
            }
            break;
        case AGENT_STATE_WAIT_FOR_GUESS:
            if (turn == FIELD_OLED_TURN_MINE) {
                ProtocolDecode(in, &enemyNegotiation, &enemyGuess);

                if (FieldGetBoatStates(&myField) != 0) {
                    turn = FIELD_OLED_TURN_MINE;
                    FieldRegisterEnemyAttack(&myField, &enemyGuess);
                    FieldOledDrawScreen(&myField, &enemyField, turn);
                    length = ProtocolEncodeHitMessage(outBuffer, &myGuess);
                    state = AGENT_STATE_SEND_GUESS;
                } else {
                    turn = FIELD_OLED_TURN_MINE;
                    FieldOledDrawScreen(&myField, &enemyField, turn);
                    length = ProtocolEncodeHitMessage(outBuffer, &myGuess);
                    state = AGENT_STATE_LOST;
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


