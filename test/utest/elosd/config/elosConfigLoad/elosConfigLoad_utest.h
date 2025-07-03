// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "elos/config/config.h"

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigLoadDefaultConfigSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigLoadDefaultDirSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigLoadInvalidConfigPathError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigLoadMultipleConfigsError)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigLoadEnvVariableConfigError)
