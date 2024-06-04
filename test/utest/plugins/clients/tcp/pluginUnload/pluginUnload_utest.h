// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/libelosplugin/types.h"

extern elosPluginConfig_t elosPluginConfig;

TEST_CASE_FUNC_PROTOTYPES(elosTestPluginUnloadSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestPluginUnloadErrInvalidPlugin)
