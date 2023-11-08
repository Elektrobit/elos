// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/plugin/plugin.h"

typedef struct elosUnitTestState {
    elosPlugin_t *plugin;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginDeleteErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginDeleteSuccess)
