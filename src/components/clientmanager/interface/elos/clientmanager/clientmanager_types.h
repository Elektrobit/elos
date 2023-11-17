// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

#include "elos/pluginmanager/types.h"

typedef struct elosClientManagerParam {
    elosPluginManager_t *pluginManager;
    samconfConfig_t *config;
} elosClientManagerParam_t;

typedef struct elosClientManager {
    elosPluginManager_t *pluginManager;
    samconfConfig_t const *config;
    const char *searchPath;
    elosPluginControlPtrVector_t pluginControlPtrVector;
} elosClientManager_t;
