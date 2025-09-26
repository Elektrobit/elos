// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

#define STO_H_DATA_LENGTH 16

__BEGIN_DECLS

/*******************************************************************
 * Magic bytes to determine if message is a DLT message
 ******************************************************************/
static const unsigned char elosDltPattern[4] = {0x44, 0x4c, 0x54, 0x01};

/*******************************************************************
 * a dlt storage header struct
 *
 * Members:
 *     seconds: time when the message was received in seconds
 *     microseconds: time when the message was received in microseconds
 *     ecuID: id of ecu which sent the dlt message
 ******************************************************************/
typedef struct elosDltStorageHeader {
    uint32_t seconds;
    int32_t microseconds;
    unsigned char ecuID[4];
} elosDltStorageHeader_t;

__END_DECLS
