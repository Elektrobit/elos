// SPDX-License-Identifier: MIT

#pragma once

#include <pthread.h>
#include <safu/ringbuffer_types.h>
#include <stddef.h>
#include <time.h>

#include "dlt_hv/types.h"
#include "elos/libelosdlt/dltmapper_types.h"

#define ELOS_DLT_MESSAGE_CODE   2042
#define ELOS_DLT_SCAN_FREQUENCY 2277

/******************************
 * The Scanner plugin instance
 *
 * Member:
 *      shmemFd: The file descriptor for with the shared memory buffer
 *      offsetAddress: The configured value from what offset the buffer should start
 *      bufferSize: The configured size of the buffer
 *      shmemFile: The file from where to get the shared memory buffer
 *      shmemLogEntries: The number of log entries the shared memory buffer can hold
 *      shmemData: The memory mapped dlt buffer
 *      localBufferCopy: A local copy of the dlt shared memory buffer
 *      idxRead: The read index for the dlt shared memory buffer
 *      parserQueue: The ring buffer to queue log entries for parsing
 *      shmemDataSize: The size of the shared memory buffer
 *      publisher: The publisher to use for publishing events
 *      scannerThread: The thread with the scanner that looks for new dlt entries
 *      stopCmd: The evntfd to tell the plugin to stop
 *      moreToRead: The eventfd for the scanner thread to tell that more events can be parsed and published
 *      sleepInterval: The time to sleep between checks in the dlt memory buffer
 ******************************/
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
    elosDltMapper_t mapper;
    struct elosPublisher *publisher;
    pthread_t scannerThread;
    int stopCmd;
    int moreToRead;
    struct timespec sleepInterval;
} elosDltScanner_t;
