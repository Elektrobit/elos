// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>
#include <tcpConfig.h>

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdConnectionLimitSuccessFromEnv)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdConnectionLimitSuccessFromDefault)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdConnectionLimitErrorLimitNotIntInEnv)
