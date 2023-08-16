// SPDX-License-Identifier: MIT
#pragma once

#include <netinet/in.h>
#include <pthread.h>
#include <safu/flags.h>
#include <samconf/samconf_types.h>
#include <stddef.h>

#include "elos/clientmanager/clientauthorization_types.h"
#include "elos/clientmanager/clientconnection_types.h"
#include "elos/eventbuffer/types.h"
#include "elos/eventdispatcher/types.h"
#include "elos/eventfilter/eventfilter_types.h"
#include "elos/eventlogging/LogAggregatorTypes.h"
#include "elos/eventprocessor/types.h"

#define CLIENT_MANAGER_MAX_CONNECTIONS     200
#define CLIENT_MANAGER_LISTEN_QUEUE_LENGTH 200

#define CLIENT_MANAGER_LISTEN_ACTIVE     (SAFU_FLAG_CUSTOM_START_BIT << 0)
#define CLIENT_MANAGER_CONNECTION_ACTIVE (SAFU_FLAG_CUSTOM_START_BIT << 1)
#define CLIENT_MANAGER_THREAD_NOT_JOINED (SAFU_FLAG_CUSTOM_START_BIT << 2)

#ifndef CLIENT_MANAGER_EVENTFILTERNODEIDVECTOR_SIZE
#define CLIENT_MANAGER_EVENTFILTERNODEIDVECTOR_SIZE 4
#endif

#ifndef CLIENT_MANAGER_EVENTQUEUEIDVECTOR_SIZE
#define CLIENT_MANAGER_EVENTQUEUEIDVECTOR_SIZE 4
#endif

typedef struct elosClientManager {
    safuFlags_t flags;
    int fd;
    int syncFd;
    struct sockaddr_in addr;
    elosClientConnection_t connection[CLIENT_MANAGER_MAX_CONNECTIONS];
    pthread_t listenThread;
    elosClientConnectionSharedData_t sharedData;
    elosClientAuthorization_t clientAuth;
} elosClientManager_t;

typedef struct elosClientManagerParam {
    samconfConfig_t *config;
    elosEventDispatcher_t *eventDispatcher;
    elosEventProcessor_t *eventProcessor;
    elosLogAggregator_t *logAggregator;
} elosClientManagerParam_t;
