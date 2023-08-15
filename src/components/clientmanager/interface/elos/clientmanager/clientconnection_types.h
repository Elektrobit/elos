// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector.h>
#include <samconf/samconf_types.h>
#include <semaphore.h>

#include "elos/eventdispatcher/types.h"
#include "elos/eventfilter/eventfilter_types.h"
#include "elos/eventlogging/LogAggregatorTypes.h"
#include "elos/eventprocessor/types.h"

typedef safuVec_t elosEventFilterNodeIdVector_t;
typedef safuVec_t elosEventQueueIdVector_t;

typedef struct elosClientConnectionSharedData {
    elosLogAggregator_t *logAggregator;
    sem_t connectionSemaphore;
    elosEventDispatcher_t *eventDispatcher;
    elosEventProcessor_t *eventProcessor;
    samconfConfig_t *config;
} elosClientConnectionSharedData_t;

typedef struct elosClientConnectionData {
    elosEventFilterNodeIdVector_t eventFilterNodeIdVector;
    elosEventQueueIdVector_t eventQueueIdVector;
} elosClientConnectionData_t;

typedef struct elosClientConnection {
    safuFlags_t flags;
    pthread_mutex_t lock;
    uint32_t status;
    int fd;
    struct sockaddr_in addr;
    pthread_t thread;
    elosClientConnectionSharedData_t *sharedData;
    elosClientConnectionData_t data;
    bool isTrusted;
    elosEventFilter_t blacklist;
    elosEventBuffer_t eventBuffer;
} elosClientConnection_t;

typedef struct elosClientConnectionParam {
    elosClientConnectionSharedData_t *sharedData;
} elosClientConnectionParam_t;
