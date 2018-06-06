#include "Protocol.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <BOARD.h>
// Datatypes

typedef enum {
    WAITING,
    RECORDING,
    FIRST_CHECKSUM_HALF,
    SECOND_CHECKSUM_HALF,
    NEWLINE
} ParseState;

typedef struct {
    char protocolMessage[PROTOCOL_MAX_MESSAGE_LEN];
    int currentIndex;
    ParseState state;
    uint8_t checksum;
} ParseData;

// Static helper functions
static unsigned char CheckSum(const char *data);
static uint8_t ASCIIToHex(char character);

// Protocol functions
ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData) {
    ParseData data;
    ProtocolParserStatus status;

    
    switch (data.state) {
    case WAITING:
        if (in == '$') { // Starts recording message when input = '$'
            data.currentIndex = 0;
            data.state = RECORDING;
            return PROTOCOL_PARSING_GOOD;
        }
        else { // If input isn't '$'
            return PROTOCOL_WAITING;
        }
        break;
    case RECORDING:
        if (in != '*') { // Keeps recording until  input = '*'
            data.protocolMessage[data.currentIndex] = in;
            data.currentIndex++;
            return PROTOCOL_PARSING_GOOD;
        }
        else { // If input is '*'
            data.state = FIRST_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case FIRST_CHECKSUM_HALF:
        if ((in >= 48 && in <= 57) || (in >= 65 && in <= 70) || (in >= 97 && in <= 102)) { // Valid hex character
            data.checksum |= ASCIIToHex(in) << 4; // Top 4 bits of checksum
            data.state = SECOND_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        }
        else {
            data.state = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    case SECOND_CHECKSUM_HALF:
        data.checksum |= ASCIIToHex(in); // Lower 4 bits of checksum
        
        if (((in >= 48 && in <= 57) || (in >= 65 && in <= 70) || (in >= 97 && in <= 102))
                && data.checksum == CheckSum(data.protocolMessage)) { // Valid hex character and matching checksums
            data.protocolMessage[data.currentIndex] = '\0';
            data.state = NEWLINE;
            return PROTOCOL_PARSING_GOOD;
        }
        else {
            data.state = WAITING;
            return PROTOCOL_PARSING_FAILURE;
        }
        break;
    case NEWLINE:        
        if (in == '\n'){ // Input is '\n'
            if (strstr(data.protocolMessage, "COO") == NULL) { // "COO,%u,%u" row, col
                strtok(data.protocolMessage, ","); // Move pointer to after MSGID
                gData->row = atoi(strtok(NULL, ","));
                gData->col = atoi(strtok(NULL, "*"));
                status = PROTOCOL_PARSED_COO_MESSAGE;
            }
            else if (strstr(data.protocolMessage, "HIT") == NULL) { // "HIT,%u,%u,%u" row, col, hit
                strtok(data.protocolMessage, ","); // Move pointer to after MSGID
                gData->row = atoi(strtok(NULL, ","));
                gData->col = atoi(strtok(NULL, ","));
                gData->hit = atoi(strtok(NULL, "*"));
                status = PROTOCOL_PARSED_HIT_MESSAGE;
            }
            else if (strstr(data.protocolMessage, "CHA") == NULL) { // "CHA,%u,%u" encryptedGuess, hash
                nData->encryptedGuess = atoi(strtok(NULL, ",")); // Move pointer to after MSGID
                nData->hash = atoi(strtok(NULL, "*"));
                status = PROTOCOL_PARSED_CHA_MESSAGE;
            }
            else if (strstr(data.protocolMessage, "DET") == NULL) { // "DET,%u,%u" guess, encryptionKey
                nData->guess = atoi(strtok(NULL, ",")); // Move pointer to after MSGID
                nData->encryptionKey = atoi(strtok(NULL, "*"));
                status = PROTOCOL_PARSED_DET_MESSAGE;
            }
            else { // If none of the messages
                data.state = WAITING;
                status = PROTOCOL_PARSING_FAILURE;
            }
        }
        else { // If input is not '\n'
            data.state = WAITING;
            status = PROTOCOL_PARSING_FAILURE;
        }
        break;
    default:
        break;
    }
    return status;
}

void ProtocolGenerateNegotiationData(NegotiationData *data) {
    data->guess = rand() % 0xFFFF; // 2^16, 16 bit number
    data->encryptionKey = rand() % 0xFFFF; // 2^16, 16 bit number
    
    data->encryptedGuess = data->guess ^ data->encryptionKey;
    data->hash = (data->encryptedGuess >> 8) ^ data->encryptedGuess;
}

uint8_t ProtocolValidateNegotiationData(const NegotiationData *data) {
    uint16_t encryptGuessTest = data->encryptionKey ^ data->guess; // Creates new encryptedGuess
    uint8_t hashTest = (data->encryptedGuess >> 8) ^ data->encryptedGuess; // Creates new hash
    
    if (encryptGuessTest == data->encryptedGuess && hashTest ==  data->hash) { // Checks if encryptedGuess and hash are the same
        return TRUE;
    }
    else {
        return FALSE;
    }
}

TurnOrder ProtocolGetTurnOrder(const NegotiationData *myData, const NegotiationData *oppData) {
    TurnOrder order;
    uint16_t turnOrder = myData ->encryptedGuess ^ oppData->encryptedGuess;
    
    int LSB = turnOrder & 1; // Gets the least significant bit
    
    if (LSB ==1) { // Player with the largest guess goes first
        if (myData ->encryptedGuess > oppData->encryptedGuess) {
            order = TURN_ORDER_START;
        }
        else if (myData ->encryptedGuess < oppData->encryptedGuess) {
            order = TURN_ORDER_DEFER;
        }
        else {
            order = TURN_ORDER_TIE;
        }
    }
    else if (LSB == 0) { // Player with the smallest guess goes first
        if (myData ->encryptedGuess < oppData->encryptedGuess) {
            order = TURN_ORDER_START;
        }
        else if (myData ->encryptedGuess > oppData->encryptedGuess) {
            order = TURN_ORDER_DEFER;
        }
        else {
            order = TURN_ORDER_TIE;
        }
    }
    return order;
}


// Calculates checksum, taken from lecture slides
unsigned char CheckSum(const char *data) {
    unsigned char outCheckSum = 0;
    int i;
    
    for (i = 0; data[i] != NULL; i++) {
        outCheckSum ^= data[i];
    }
    return outCheckSum;
}

// Converts ASCII hex character to uint8_t
uint8_t ASCIIToHex(char character) {
    uint8_t hexNum;
    
    if (character >= 48 && character <= 57) { // 0-9
        hexNum = (character - 48);
    }
    else if (character >= 65 && character <= 70) { // A-F
        hexNum = (character - 55);
    }
    else if (character >= 97 && character <= 102) { // a-f
        hexNum = (character - 87);
    }
    return hexNum;
}