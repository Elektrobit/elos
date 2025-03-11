// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <mock_libelosplugin.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "elos/libelosplugin/types.h"

extern elosPluginConfig_t elosPluginConfig;

#define ELOS_KMSG_FILE_KEY_MOCK     "ELOS_KMSG_FILE"
#define ELOS_KMSG_FILE_MOCK         "/dev/kmsg"
#define ELOS_KMSGSTATEFILE_KEY_MOCK "ELOS_KMSGSTATEFILE"
#define ELOS_KMSGSTATEFILE_MOCK     "/tmp/kmsgStateFile"

TEST_CASE_FUNC_PROTOTYPES(elosTestPluginLoadSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestPluginLoadErrInvalidPlugin)
