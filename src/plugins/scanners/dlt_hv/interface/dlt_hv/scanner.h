// SPDX-License-Identifier: MIT

#pragma once

#include <bits/pthreadtypes.h>
#include <stddef.h>
#include <time.h>

#define NUM_BUFFER 3

typedef struct elosDltScanner {
    int shmemFd;
    char *shmemFile;
    size_t shmemLogEntries;
    size_t shmemOffset;
    void *shmemData;
    void *localData[NUM_BUFFER];
    size_t bufferIdx;
    size_t shmemDataSize;
    struct elosPublisher *publisher;
    pthread_t scannerThread;
    int stopCmd;
    int moreToRead;
    struct timespec sleepInterval;
} elosDltScanner_t;
