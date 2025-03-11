// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <mock_plugincontrol.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "elos/libelosplugin/types.h"
#include "mock_kmsg_mapper.h"

extern elosPluginConfig_t elosPluginConfig;

#define MOCK_FD 42

TEST_CASE_FUNC_PROTOTYPES(elosTestPluginUnloadSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestPluginUnloadErrInvalidPlugin)

typedef struct elosTestState {
    struct elosKmsgContext context;
} elosTestState_t;