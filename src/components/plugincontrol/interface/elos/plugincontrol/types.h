// SPDX-License-Identifier: MIT
#pragma once

#include <elos/libelosplugin/types.h>
#include <pthread.h>
#include <safu/flags.h>
#include <safu/result.h>
#include <safu/vector_types.h>
#include <samconf/samconf_types.h>

#include "elos/common/types.h"

#define ELOS_PLUGIN_FLAG_WORKERRUNNING              SAFU_FLAG_CUSTOM_START_BIT
#define ELOS_PLUGIN_FLAG_HAS_WORKERRUNNING_BIT(__f) ((atomic_load(__f) & ELOS_PLUGIN_FLAG_WORKERRUNNING) != 0)

typedef enum elosPluginControlFuncEntryE {
    ELOS_PLUGIN_FUNC_LOAD = 0,
    ELOS_PLUGIN_FUNC_START,
    ELOS_PLUGIN_FUNC_STOP,
    ELOS_PLUGIN_FUNC_UNLOAD,
    ELOS_PLUGIN_FUNC_COUNT,
} elosPluginControlFuncEntryE_t;

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
    elosPluginControlParamFuncOverride_t funcOverride;
    char const *path;
    char const *file;
    void *data;
} elosPluginControlParam_t;

typedef struct elosPluginControl {
    safuFlags_t flags;
    elosPluginControlFuncEntry_t func[ELOS_PLUGIN_FUNC_COUNT];
    elosPlugin_t context;
    pthread_t workerThread;
    int sync;
    char const *path;
    char *file;
    void *dlHandle;
} elosPluginControl_t;

typedef safuVec_t elosPluginControlVector_t;
typedef safuVec_t elosPluginControlPtrVector_t;
