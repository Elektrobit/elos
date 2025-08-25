// SPDX-License-Identifier: MIT

#pragma once

#include <pthread.h>
#include <safu/ringbuffer_types.h>
#include <stddef.h>
#include <time.h>

#include "dlt_hv/types.h"

#define ELOS_DLT_MESSAGE_CODE 2042

typedef struct elosDltScanner {
    int shmemFd;
    size_t offsetAddress;
    size_t bufferSize;
    char *shmemFile;
    size_t shmemLogEntries;
    void *shmemData;
    elosEbLogRingBuffer_t *localBufferCopy;
    uint16_t idxRead;
    safuRingBuffer_t parserQueue;
    size_t shmemDataSize;
    struct elosPublisher *publisher;
    pthread_t scannerThread;
    int stopCmd;
    int moreToRead;
    struct timespec sleepInterval;
} elosDltScanner_t;
