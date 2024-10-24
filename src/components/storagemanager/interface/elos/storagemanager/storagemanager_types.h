// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf_types.h>

#include "elos/pluginmanager/types.h"
#include "elos/storagemanager/StorageBackend.h"

/*******************************************************************
 * Initialization parameters for a new storage manager component
 *
 * Members:
 *   pluginManager: plugin manager where storage plugins are stored and managed
 *   config: elos configuration variables
 ******************************************************************/
typedef struct elosStorageManagerParam {
    elosPluginManager_t *pluginManager;
    samconfConfig_t *config;
} elosStorageManagerParam_t;

/*******************************************************************
 * Data structure of a StorageManager
 *
 * Members:
 *   pluginManager: component to manage storage plugins
 *   config: configuration variables for elos
 *   searchPath: string for the plugin manager to start search from
 *   pluginControlPtrVector: Plugin Control vector to load storage plugins on to
 ******************************************************************/
typedef struct elosStorageManager {
    elosPluginManager_t *pluginManager;
    samconfConfig_t const *config;
    const char *searchPath;
    elosPluginControlVector_t pluginControlPtrVector;
    elosStorageBackendPtrVector_t backends;
} elosStorageManager_t;
