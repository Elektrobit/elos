// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "unix_config/config.h"

samconfConfig_t elosGetMockConfig();

TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetPathSuccessFromConfig)
TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetPathSuccessFromDefault)
