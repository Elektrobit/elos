// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

#define ELOS_EB_LOG_STRING_SIZE 128

/***********************************
 * The log level for a DLT entry
 ***********************************/
typedef enum elosDltLogLevelE {
    ELOS_DLT_LOGLEVEL_OFF = 1,
    ELOS_DLT_LOGLEVEL_FATAL,
    ELOS_DLT_LOGLEVEL_ERROR,
    ELOS_DLT_LOGLEVEL_WARN,
    ELOS_DLT_LOGLEVEL_INFO,
    ELOS_DLT_LOGLEVEL_DEBUG,
    ELOS_DLT_LOGLEVEL_DEFAULT = ELOS_DLT_LOGLEVEL_INFO,
} elosDltLogLevelE_t;

/***********************************
 * Represents a DLT entry
 *
 * Members:
 *    creationTime: The time of creation of this entry as UNIX timestamp
 *    producerId: The id for the producer of this entry
 *    logLevel: The log level of this entry
 *    pad: 6 bytes of padding for memory alignment
 *    logString: The payload of the entry
 ***********************************/
typedef struct elosEbLogEntry {
    uint64_t creationTime;
    uint8_t producerId;
    uint8_t logLevel;
    uint8_t pad[6];
    char logString[ELOS_EB_LOG_STRING_SIZE];
} elosEbLogEntry_t;

/***********************************
 * Represents the shared memory dlt ring buffer
 *
 * Members:
 *    idxWrite: The write index where the next entry will be written
 *    idxRead: The read index only used at first opening of the buffer
 *    entryCount: The number of entries in the ring buffer
 *    pad: Some padding for memory alignment
 *    entries: The buffer that holds the dlt entries
 ***********************************/
typedef struct elosEbLogRingBuffer {
    uint16_t idxWrite;
    uint16_t idxRead;
    uint16_t entryCount;
    uint16_t pad;
    elosEbLogEntry_t entries[];
} elosEbLogRingBuffer_t;
