// SPDX-License-Identifier: MIT
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "elos/libelosdlt/message_types.h"

#define EXT_H_DATA_LENGTH 10

#define VERBOSE_MASK           0x01
#define MESSAGE_TYPE_MASK      0x0E
#define MESSAGE_TYPE_INFO_MASK 0xF0

#define _MESSAGE_TYPE_SHIFT      1
#define _MESSAGE_TYPE_INFO_SHIFT 4

__BEGIN_DECLS

/*******************************************************************
 * struct to hold the message type and message type info converted to string.
 *
 * Members:
 *     isVerbose: string form of dlt ext header verbose flag
 *     messageTypeName: string form of dlt ext header messageType
 *     messageTypeInfoName: string form of dlt ext header messageTypeInfo
 ******************************************************************/
typedef struct elosDltMessageName {
    char *isVerbose;
    char *messageTypeName;
    char *messageTypeInfoName;
} elosDltMessageName_t;

/*******************************************************************
 * struct to hold the message type info of a dlt extended header.
 *
 * Members:
 *     verbose: flag for setting verbose or non-verbose mode of data transfer.
 *     messageType: the type of message transmitted.
 *     messageTypeInfo: the sub type of message transmitted.
 ******************************************************************/
typedef struct elosDltMessageInfo {
    bool verbose;
    elosDltMessageTypeE_t messageType;
    union {
        elosDltMessageLogInfoE_t logInfo;
        elosDltMessageTraceInfoE_t traceInfo;
        elosDltMessageNetworkInfoE_t networkInfo;
        elosDltMessageControlInfoE_t controlInfo;
    } messageTypeInfo;
} elosDltMessageInfo_t;

/*******************************************************************
 * a dlt extended header struct
 *
 * Members:
 *     messageInfo: information about type and mode of message being transmitted.
 *     noOfArguments: number of arguments provided in payload.
 *     applicationId: abbreviation of application generating dlt message
 *     contextId: user defined id created to group dlt messages.
 ******************************************************************/
typedef struct elosDltExtHeader {
    elosDltMessageInfo_t messageInfo;
    uint8_t noOfArguments;
    unsigned char applicationId[4];
    unsigned char contextId[4];
} elosDltExtHeader_t;

__END_DECLS
