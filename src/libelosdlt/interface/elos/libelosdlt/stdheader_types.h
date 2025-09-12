// SPDX-License-Identifier: MIT
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define STD_H_DATA_LENGTH 4

#define USE_EXTENDED_HEADER_MASK 0x01
#define MSB_FIRST_MASK           0x02
#define WITH_ECU_ID_MASK         0x04
#define WITH_SESSION_ID_MASK     0x08
#define WITH_TIME_STAMP_MASK     0x16
#define VERSION_NUMBER_MASK      0xE0

__BEGIN_DECLS

/*******************************************************************
 * a dlt header type struct
 *
 * Members:
 *     ueh: use extended header flag
 *     msbf: most significant byte first while parsing payload flag
 *     weid: with ecuID flag
 *     wsid: with session id flag
 *     wtms: with timestamp
 *     vers: dlt protocol version
 ******************************************************************/
typedef struct elosDltStdHeaderType {
    bool ueh;
    bool msbf;
    bool weid;
    bool wsid;
    bool wtms;
    uint8_t vers : 3;
} elosDltStdHeaderType_t;

/*******************************************************************
 * a dlt standard header struct
 *
 * Members:
 *     headerType: general information about dlt message.
 *     messageCounter: counts the number of messages received by the dlt module
 *     length: sum of length of standard, extended headers and payload
 *     ecuID: id of ecu which sent the dlt message
 *     sessionId: id of the source which sent the dlt message
 *     timestamp: time of dlt message generation.
 ******************************************************************/
typedef struct elosDltStdHeader {
    elosDltStdHeaderType_t headerType;
    uint8_t messageCounter;
    uint16_t length;
    unsigned char ecuID[4];
    uint32_t sessionId;
    uint32_t timestamp;
} elosDltStdHeader_t;

__END_DECLS
