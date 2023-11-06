// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>

#include "elos/common/types.h"

#define ELOS_PLUGIN_ID_INVALID 0

#define ELOS_PLUGIN_FLAG_WORKERRUNNING              SAFU_FLAG_CUSTOM_START_BIT
#define ELOS_PLUGIN_FLAG_HAS_WORKERRUNNING_BIT(__f) ((atomic_load(__f) & ELOS_PLUGIN_FLAG_WORKERRUNNING) != 0)

typedef elosId_t elosPluginId_t;
typedef uint32_t elosPluginState_t;

typedef enum elosPluginStateE {
    PLUGIN_STATE_INVALID = 0,
    PLUGIN_STATE_INITIALIZED,
    PLUGIN_STATE_LOADED,
    PLUGIN_STATE_STARTED,
    PLUGIN_STATE_STOPPED,
    PLUGIN_STATE_UNLOADED,
    PLUGIN_STATE_ERROR,
} elosPluginStateE_t;

typedef struct elosPluginContext {
    elosPluginStateE_t state;
    samconfConfig_t const *config;
    elosPluginId_t id;
    void *data;
    int sync;
    int stop;
} elosPluginContext_t;

typedef safuResultE_t(elosPluginFunc_t)(elosPluginContext_t *);

typedef enum elosPluginFuncEntryE {
    ELOS_PLUGIN_FUNC_LOAD = 0,
    ELOS_PLUGIN_FUNC_START,
    ELOS_PLUGIN_FUNC_STOP,
    ELOS_PLUGIN_FUNC_UNLOAD,
    ELOS_PLUGIN_FUNC_COUNT,
} elosPluginFuncEntryE_t;

typedef struct elosPluginFuncEntry {
    char *name;
    elosPluginFunc_t *ptr;
} elosPluginFuncEntry_t;

typedef struct elosPluginParamFuncOverride {
    char const *load;
    char const *unload;
    char const *start;
    char const *stop;
} elosPluginParamFuncOverride_t;

typedef struct elosPluginParam {
    elosPluginId_t id;
    samconfConfig_t const *config;
    elosPluginParamFuncOverride_t funcOverride;
    char const *path;
    char const *file;
    void *data;
} elosPluginParam_t;

typedef struct elosPlugin {
    safuFlags_t flags;
    elosPluginFuncEntry_t func[ELOS_PLUGIN_FUNC_COUNT];
    elosPluginContext_t context;
    pthread_t workerThread;
    int sync;
    char const *path;
    char *file;
    void *dlHandle;
} elosPlugin_t;

typedef safuVec_t elosPluginVector_t;
typedef safuVec_t elosPluginPtrVector_t;
