/* SPDX-License-Identifier: MIT */
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/plugin/plugin.h"

typedef struct elosUnitTestState {
    elosPlugin_t plugin;
} elosUnitTestState_t;

extern char const *_funcDefaultName[ELOS_PLUGIN_FUNC_COUNT];

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginInitializeErrFunc)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginInitializeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginInitializeSuccessFuncOverride)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginInitializeSuccess)
