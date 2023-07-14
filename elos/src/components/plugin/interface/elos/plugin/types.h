// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/result.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>

#include "elos/common/types.h"

#define ELOS_PLUGIN_ID_INVALID 0

typedef elosId_t elosPluginId_t;
typedef uint32_t elosPluginState_t;

typedef safuResultE_t(elosPluginFunc_t)(void *);

typedef enum elosPluginStateE {
    PLUGIN_STATE_INVALID = 0,
    PLUGIN_STATE_INITIALIZED,
    PLUGIN_STATE_LOADED,
    PLUGIN_STATE_STARTED,
    PLUGIN_STATE_STOPPED,
    PLUGIN_STATE_UNLOADED,
    PLUGIN_STATE_ERROR,
} elosPluginStateE_t;

typedef struct elosPluginFuncTable {
    elosPluginFunc_t *load;
    elosPluginFunc_t *unload;
    elosPluginFunc_t *start;
    elosPluginFunc_t *stop;
} elosPluginFuncTable_t;

typedef struct elosPluginParam {
    elosPluginId_t id;
    samconfConfig_t const *config;
    char const *path;
    void *loadParam;
    void *data;
} elosPluginParam_t;

typedef struct elosPluginWorker {
    pthread_t thread;
    bool isThreadRunning;
    int sync;
} elosPluginWorker_t;

typedef struct elosPlugin {
    elosPluginId_t id;
    elosPluginStateE_t state;
    elosPluginFuncTable_t func;
    samconfConfig_t const *config;
    elosPluginWorker_t worker;
    int sync;
    int stop;
    void *data;
    char const *path;
    char *file;
    void *dlHandle;
} elosPlugin_t;

typedef safuVec_t elosPluginVector_t;
typedef safuVec_t elosPluginPtrVector_t;
