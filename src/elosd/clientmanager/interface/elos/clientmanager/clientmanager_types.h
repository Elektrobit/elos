// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

#include "elos/pluginmanager/types.h"

/*******************************************************************
 * Initialization parameters for a new client manager component
 *
 * Members:
 *   pluginManager: plugin manager where client plugins are stored and managed
 *   config: elos configuration variables
 ******************************************************************/
typedef struct elosClientManagerParam {
    elosPluginManager_t *pluginManager;
    samconfConfig_t *config;
} elosClientManagerParam_t;

/*******************************************************************
 * Data structure of a ClientManager
 *
 * Members:
 *   pluginManager: component to manage client plugins
 *   config: configuration variables for elos
 *   searchPath: string for the plugin manager to start search from
 *   pluginControlPtrVector: Plugin Control vector to load client plugins on to
 ******************************************************************/
typedef struct elosClientManager {
    elosPluginManager_t *pluginManager;
    samconfConfig_t const *config;
    const char *searchPath;
    elosPluginControlPtrVector_t pluginControlPtrVector;
} elosClientManager_t;
