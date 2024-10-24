// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

#include "elos/pluginmanager/types.h"

/*******************************************************************
 * Initialization parameters for a new scanner manager component
 *
 * Members:
 *   pluginManager: plugin manager where scanner plugins are stored and managed
 *   config: elos configuration variables
 ******************************************************************/
typedef struct elosScannerManagerParam {
    elosPluginManager_t *pluginManager;
    samconfConfig_t *config;
} elosScannerManagerParam_t;

/*******************************************************************
 * Data structure of a ScannerManager
 *
 * Members:
 *   pluginManager: component to manage scanner plugins
 *   config: configuration variables for elos
 *   searchPath: string for the plugin manager to start search from
 *   pluginControlPtrVector: Plugin Control vector to load scanner plugins on to
 ******************************************************************/
typedef struct elosScannerManager {
    elosPluginManager_t *pluginManager;
    samconfConfig_t const *config;
    const char *searchPath;
    elosPluginControlVector_t pluginControlPtrVector;
} elosScannerManager_t;
