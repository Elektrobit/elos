// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <elos/plugin/plugin.h>

#include "elos/plugin/types.h"

typedef struct elosPluginTest {
    elosPlugin_t plugin;
} elosPluginTest_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginGetNameErrInvalidParameter)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginGetNameSuccess)
