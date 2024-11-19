// SPDX-License-Identifier: MIT

#include <stdint.h>
#include <stdlib.h>

#include "elos/lite/event.h"

#define ELOS_PROTOCOL_VERSION               0x01
#define ELOS_MESSAGE_EVENT_PUBLISH          0x02
#define ELOS_MESSAGE_RESPONSE_BIT           0x80
#define ELOS_MESSAGE_RESPONSE_EVENT_PUBLISH (ELOS_MESSAGE_EVENT_PUBLISH | ELOS_MESSAGE_RESPONSE_BIT)

/*******************************************************************
 * The elos protocol header
 ******************************************************************/
struct elosliteMessageHead {
    uint8_t version;
    uint8_t message;
    uint16_t length;
};

/*******************************************************************
 * holds a small buffer for sending events
 ******************************************************************/
typedef struct elosliteBuffer {
    char *buffer;
    size_t size;
    size_t pos;
} elosliteBuffer_t;

/*******************************************************************
 * Gets the number of digits of a number in base 10
 *
 * Args:
 *      num: The number to get the length of
 *
 * Returns:
 *      The number of digits num has in base 10
 ******************************************************************/
uint16_t elosliteNumLen(int64_t num);

/*******************************************************************
 * Returns the length of an event will have after serialization.
 * Without actually doing the serializing.
 *
 * Args:
 *      event: The event to get the serialized length of
 * Returns:
 *      The lenght the event will have
 ******************************************************************/
uint16_t elosliteMsgLen(elosliteEvent_t *event);
