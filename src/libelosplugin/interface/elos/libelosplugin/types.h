// SPDX-License-Identifier: MIT
#pragma once

#include <safu/flags.h>
#include <safu/result.h>
#include <samconf/samconf.h>

#define ELOS_PLUGIN_ID_INVALID 0

typedef uint32_t elosPluginId_t;
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
