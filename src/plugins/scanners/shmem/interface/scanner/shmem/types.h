// SPDX-License-Identifier: MIT
#pragma once

#include <stdint.h>

#define ELOS_EB_LOG_STRING_SIZE 128

typedef enum elosDltLogLevelE {
    ELOS_DLT_LOGLEVEL_OFF = 1,
    ELOS_DLT_LOGLEVEL_FATAL,
    ELOS_DLT_LOGLEVEL_ERROR,
    ELOS_DLT_LOGLEVEL_WARN,
    ELOS_DLT_LOGLEVEL_INFO,
    ELOS_DLT_LOGLEVEL_DEBUG,
    ELOS_DLT_LOGLEVEL_DEFAULT = ELOS_DLT_LOGLEVEL_INFO,
} elosDltLogLevelE_t;

typedef struct elosEbLogEntry {
    uint64_t creationTime;
    uint8_t producerId;
    uint8_t logLevel;
    uint8_t pad[6];
    char logString[ELOS_EB_LOG_STRING_SIZE];
} elosEbLogEntry_t;

typedef struct elosEbLogRingBuffer {
    uint16_t idxWrite;
    uint16_t idxRead;
    uint16_t entryCount;
    uint16_t pad;
    elosEbLogEntry_t entries[];
} elosEbLogRingBuffer_t;
