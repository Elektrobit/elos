// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/types.h>
#include <pthread.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>

#include "elos/common/types.h"
#include "elos/eventbuffer/types.h"
#include "elos/eventdispatcher/types.h"
#include "elos/eventlogging/LogAggregatorTypes.h"
#include "elos/eventprocessor/types.h"

__BEGIN_DECLS

typedef struct elosPluginControlFuncEntry {
    char *name;
    elosPluginFunc_t *ptr;
} elosPluginControlFuncEntry_t;

__END_DECLS

typedef struct elosPluginControlParamFuncOverride {
    char const *load;
    char const *unload;
    char const *start;
    char const *stop;
} elosPluginControlParamFuncOverride_t;

typedef struct elosPluginControlParam {
    elosPluginId_t id;
    samconfConfig_t const *config;
    elosPluginTypeE_t pluginType;
    char const *path;
    char const *file;
    void *data;
    elosEventProcessor_t *eventProcessor;
    elosEventDispatcher_t *eventDispatcher;
    struct elosLogAggregator *logAggregator;
} elosPluginControlParam_t;

typedef struct elosPublisher {
    elosEventBuffer_t eventBuffer;
    struct elosLogAggregator *logAggregator;
} elosPublisher_t;

typedef struct elosSubscriber {
    safuVec_t subscriptions;
    elosEventProcessor_t *eventProcessor;
} elosSubscriber_t;

typedef struct elosPluginControl {
    safuFlags_t flags;
    elosPluginConfig_t const *pluginConfig;
    elosPluginTypeE_t pluginType;
    elosPlugin_t context;
    pthread_t workerThread;
    int sync;
    char const *path;
    char *file;
    void *dlHandle;
    elosEventProcessor_t *eventProcessor;
    elosEventDispatcher_t *eventDispatcher;
    struct elosLogAggregator *logAggregator;
} elosPluginControl_t;

typedef safuVec_t elosPluginControlVector_t;
typedef safuVec_t elosPluginControlPtrVector_t;
