#include "Protocol.h"
#include <stdlib.h>

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
static uint8_t CharToInt();

// Protocol Functions

int ProtocolEncodeCooMessage(char *message, const GuessData *data)
{
    return sprintf(message, PAYLOAD_TEMPLATE_COO, data -> row, data ->col);  // "COO,%u,%u"
}

int ProtocolEncodeHitMessage(char *message, const GuessData *data)
{ 
    return sprintf(message, PAYLOAD_TEMPLATE_HIT, data -> row, data ->col, data ->hit); // "HIT,%u,%u,%u"
}

int ProtocolEncodeChaMessage(char *message, const NegotiationData *data)
{ 
    return sprintf(message, PAYLOAD_TEMPLATE_CHA, data ->encryptedGuess, data ->hash); // "CHA,%u,%u"
}

int ProtocolEncodeDetMessage(char *message, const NegotiationData *data)
{ 
    return sprintf(message, PAYLOAD_TEMPLATE_DET, data ->guess, data ->encryptionKey); // "DET,%u,%u"
}

ProtocolParserStatus ProtocolDecode(char in, NegotiationData *nData, GuessData *gData) {
    switch (ParseData ->state) {
    case WAITING:
        if (char == '$') {
            ParseData ->currentIndex = 0;
            ParseData ->state = RECORDING;
            return PROTOCOL_PARSING_GOOD;
        }
        else {
            return PROTOCOL_WAITING;
        }
        break;
    case RECORDING:
        if (char == '*') {
            ParseData ->state = FIRST_CHECKSUM_HALF;
            return PROTOCOL_PARSING_GOOD;
        }
        else {
            ParseData ->protocolMessage[ParseData ->currentIndex] = in;
            ParseData ->currentIndex++;
            return PROTOCOL_PARSING_GOOD;
        }
        break;
    case FIRST_CHECKSUM_HALF:
        
        break;
    case SECOND_CHECKSUM_HALF:
        break;
    case NEWLINE:
        break;
    }
}

// Calculates checksum, taken from lecture slides
static unsigned char CheckSum(const char *data) {
    unsigned char outCheckSum = 0;
    int i;
    
    for (i = 0; data[i] != NULL; i++) {
        outCheckSum ^= data[i];
    }
    return outCheckSum;
}

static uint8_t CharToInt(char character) {
    
}