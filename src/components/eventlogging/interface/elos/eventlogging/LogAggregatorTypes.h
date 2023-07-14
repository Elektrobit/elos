// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

#include "elos/pluginmanager/types.h"

typedef struct elosLogAggregatorParam {
    elosPluginManager_t *pluginManager;
    samconfConfig_t *config;
} elosLogAggregatorParam_t;

typedef struct elosLogAggregator {
    elosPluginManager_t *pluginManager;
    pthread_mutex_t *lock;
    samconfConfig_t const *config;
    elosPluginPtrVector_t pluginPtrVector;
} elosLogAggregator_t;
