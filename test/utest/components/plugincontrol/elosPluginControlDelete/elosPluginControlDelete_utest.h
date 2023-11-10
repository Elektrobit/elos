// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/plugincontrol/plugincontrol.h"

typedef struct elosUnitTestState {
    elosPluginControl_t *plugin;
} elosUnitTestState_t;

TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlDeleteErrParam)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosPluginControlDeleteSuccess)
