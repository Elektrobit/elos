// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/types.h>
#include <pthread.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>

#include "elos/common/types.h"

typedef struct elosPluginControlFuncEntry {
    char *name;
    elosPluginFunc_t *ptr;
} elosPluginControlFuncEntry_t;

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
} elosPluginControlParam_t;

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
} elosPluginControl_t;

typedef safuVec_t elosPluginControlVector_t;
typedef safuVec_t elosPluginControlPtrVector_t;
