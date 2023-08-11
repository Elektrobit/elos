// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include <samconf/samconf_types.h>
#include <semaphore.h>
#include <stddef.h>

#include "elos/clientmanager/clientauthorization_types.h"
#include "elos/eventbuffer/types.h"
#include "elos/eventdispatcher/types.h"
#include "elos/eventfilter/eventfilter_types.h"
#include "elos/eventlogging/LogAggregatorTypes.h"
#include "elos/eventprocessor/types.h"

#define CLIENT_MANAGER_MAX_CONNECTIONS     200
#define CLIENT_MANAGER_LISTEN_QUEUE_LENGTH 200
#define CLIENT_MANAGER_LISTEN_ACTIVE       1
#define CLIENT_MANAGER_CONNECTION_ACTIVE   1
#define CLIENT_MANAGER_THREAD_NOT_JOINED   2

#ifndef CLIENT_MANAGER_EVENTFILTERNODEIDVECTOR_SIZE
#define CLIENT_MANAGER_EVENTFILTERNODEIDVECTOR_SIZE 4
#endif

#ifndef CLIENT_MANAGER_EVENTQUEUEIDVECTOR_SIZE
#define CLIENT_MANAGER_EVENTQUEUEIDVECTOR_SIZE 4
#endif

typedef safuVec_t elosEventFilterNodeIdVector_t;
typedef safuVec_t elosEventQueueIdVector_t;

typedef struct elosClientManagerSharedData {
    elosLogAggregator_t *logAggregator;
    sem_t connectionSemaphore;
    elosEventDispatcher_t *eventDispatcher;
    elosEventProcessor_t *eventProcessor;
} elosClientManagerSharedData_t;

typedef struct elosClientManagerData {
    elosEventFilterNodeIdVector_t eventFilterNodeIdVector;
    elosEventQueueIdVector_t eventQueueIdVector;
} elosClientManagerData_t;

typedef struct elosClientManagerConnection {
    pthread_mutex_t lock;
    uint32_t status;
    int fd;
    struct sockaddr_in addr;
    pthread_t thread;
    elosClientManagerSharedData_t *sharedData;
    elosClientManagerData_t data;
    bool isTrusted;
    elosEventFilter_t blacklist;
    elosEventBuffer_t eventBuffer;
} elosClientManagerConnection_t;

typedef struct elosClientManagerContext {
    pthread_mutex_t lock;
    uint32_t status;
    int fd;
    struct sockaddr_in addr;
    elosClientManagerConnection_t connection[CLIENT_MANAGER_MAX_CONNECTIONS];
    pthread_t listenThread;
    elosClientManagerSharedData_t sharedData;
    elosClientAuthorization_t clientAuth;
} elosClientManagerContext_t;

typedef struct elosClientManagerParam {
    samconfConfig_t *config;
    elosEventDispatcher_t *eventDispatcher;
    elosEventProcessor_t *eventProcessor;
    elosLogAggregator_t *logAggregator;
} elosClientManagerParam_t;
