// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "tcp_config/config.h"

samconfConfig_t elosGetMockConfig();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdPortSuccessFromConfig)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdPortSuccessFromDefault)
