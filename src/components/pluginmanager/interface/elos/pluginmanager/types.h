// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/types.h"

#ifndef PLUGINMANAGER_PLUGINVECTOR_INIT_SIZE
#define PLUGINMANAGER_PLUGINVECTOR_INIT_SIZE 8
#endif

typedef enum elosPluginManagerStateE {
    PLUGINMANAGER_STATE_INVALID = 0,
    PLUGINMANAGER_STATE_INITIALIZED,
} elosPluginManagerStateE_t;

typedef struct elosPluginManagerParam {
    samconfConfig_t *config;
    elosEventProcessor_t *eventProcessor;
    elosEventDispatcher_t *eventDispatcher;
    struct elosLogAggregator *logAggregator;
} elosPluginManagerParam_t;

typedef struct elosPluginManager {
    elosPluginManagerStateE_t state;
    elosPluginControlVector_t pluginVector;
    samconfConfig_t *config;
    bool useEnv;
    char const *errStr;
    elosPluginId_t nextId;  // Placeholder that should be replaced with idManager in the future
    elosEventProcessor_t *eventProcessor;
    elosEventDispatcher_t *eventDispatcher;
    struct elosLogAggregator *logAggregator;
} elosPluginManager_t;

typedef safuVec_t elosPluginIdVector_t;
typedef safuResultE_t(elosPluginManagerLoadCallback_t)(elosPluginManager_t *, elosPluginId_t, void *);
