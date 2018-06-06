#include "Protocol.h"
#include "Field.h"
#include "Agent.h"
#include "BOARD.h"

//Static function
    static Field *myField;
    static Field *enemyField;
    static NegotiationData *myNegotiation;
    static NegotiationData *enemyNegotiation;
    static GuessData *myGuess;
    static GuessData *enemyGuess;
    static AgentEvent event;
    static TurnData turn;
    
    
typedef enum {
    MINE,
    THEIRS,
    NONE
} TurnData;    

/**
 * The Init() function for an Agent sets up everything necessary for an agent before the game
 * starts. This can include things like initialization of the field, placement of the boats,
 * etc. The agent can assume that stdlib's rand() function has been seeded properly in order to
 * use it safely within.
 */

void AgentInit(void) {
    FieldInit(&myField, FIELD_POSITION_EMPTY);
    FieldInit(&enemyField, FIELD_POSITION_EMPTY);
    
    uint8_t row = rand() % 6; // random row
    uint8_t col = rand() % 10; // random column
    BoatDirection dir = rand() % FIELD_BOAT_DIRECTION_WEST; // random direction
    
    // My field
    FieldAddBoat(myField, row, col, dir, FIELD_BOAT_SMALL);
    FieldAddBoat(myField, row, col, dir, FIELD_BOAT_MEDIUM);
    FieldAddBoat(myField, row, col, dir, FIELD_BOAT_LARGE);
    FieldAddBoat(myField, row, col, dir, FIELD_BOAT_HUGE);
    
    // Enemy field
    FieldAddBoat(enemyField, row, col, dir, FIELD_BOAT_SMALL);
    FieldAddBoat(enemyField, row, col, dir, FIELD_BOAT_MEDIUM);
    FieldAddBoat(enemyField, row, col, dir, FIELD_BOAT_LARGE);
    FieldAddBoat(enemyField, row, col, dir, FIELD_BOAT_HUGE);
}

int AgentRun(char in, char *outBuffer) {
    AgentState state = AGENT_STATE_GENERATE_NEG_DATA;
    int length = 0;
    
    switch (state) {
    case AGENT_STATE_GENERATE_NEG_DATA:
        ProtocolGenerateNegotiationData(myNegotiation); //Generate NegotiationData for my field
        length = ProtocolEncodeChaMessage(outBuffer, myNegotiation); // Encodes challenge message and output it to buffer
        state = AGENT_STATE_SEND_CHALLENGE_DATA;
        break;
    case AGENT_STATE_SEND_CHALLENGE_DATA:
        if (event == AGENT_EVENT_RECEIVED_CHA_MESSAGE) {
            ProtocolDecode(in, enemyNegotiation);
            length = ProtocolEncodeDetMessage(outBuffer, myNegotiation);
            state = AGENT_STATE_DETERMINE_TURN_ORDER;
        }
        break;
    case AGENT_STATE_DETERMINE_TURN_ORDER:
        if (event == AGENT_EVENT_RECEIVED_DET_MESSAGE) {
            if (ProtocolValidateNegotiationData(enemyNegotiation == TRUE)) { // If opponent negotiation data is valid
                if (ProtocolGetTurnOrder(enemyNegotiation, enemyNegotiation == TURN_ORDER_START)) { // If you won turn ordering
                    turn = MINE;
                    OledUpdate();
                    state = AGENT_STATE_SEND_GUESS;
                
                }
                else if (ProtocolGetTurnOrder(myNegotiation, enemyNegotiation == TURN_ORDER_DEFER)) { // If you didn't win turn ordering
                    turn = THEIRS;
                    OledUpdate();
                    state = AGENT_STATE_WAIT_FOR_GUESS;
                }
                else { // If turn ordering was a tie
                    OledClear(OLED_COLOR_BLACK);
                    // Set OLED to ERROR_STRING_ORDERING
                }
            }
            else { // If opponent negotiation data is invalid
                OledClear(OLED_COLOR_BLACK);
                // Set OLED to ERROR_STRING_ORDERING
                state = AGENT_STATE_INVALID;
            }
        }
        break;
    case AGENT_STATE_SEND_GUESS:
        if (turn == MINE) {
            myGuess ->row = rand() % 6;
            myGuess->col = rand() % 10;
            length = ProtocolEncodeCooMessage(outBuffer, myGuess);
            state = AGENT_STATE_WAIT_FOR_HIT;
        }
        break;
    case AGENT_STATE_WAIT_FOR_HIT:
        ProtocolDecode(in, enemyNegotiation, enemyGuess); // Not sure whether to store in my data or enemy data
        
        if (FieldGetBoatStates(enemyField) =! 0) {
            FieldUpdateKnowledge(enemyField, myGuess);
            turn = THEIRS;
            OledUpdate();
            state = AGENT_STATE_WAIT_FOR_GUESS;
        }
        else {
            turn = NONE;
            OledUpdate();
            state = AGENT_STATE_WON;
        }
        break;
    case AGENT_STATE_WAIT_FOR_GUESS:
        if (turn == THEIRS) {
            ProtocolDecode(in, enemyNegotiation, enemyGuess);
            
            if (FieldGetBoatStates(myField) =! 0) {
                turn = MINE;
                FieldRegisterEnemyAttack(myField, enemyGuess);
                OledUpdate();
                length = ProtocolEncodeHitMessage(outBuffer, myNegotiation);
                state = AGENT_STATE_SEND_GUESS;
             }
            else {
                turn = NONE;
                OledUpdate();
                length = ProtocolEncodeHitMessage(outBuffer, myNegotiation);
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

