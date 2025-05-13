// SPDX-License-Identifier: MIT
#ifndef ELOS_CONFIG_GET_ELOSD_LOGLEVEL_UTEST_H
#define ELOS_CONFIG_GET_ELOSD_LOGLEVEL_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

#include "elos/config/config.h"

samconfConfig_t elosGetMockConfig();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdLogLevelSuccessFromDefault)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdLogLevelSuccessFromConfig)

#endif /* ELOS_CONFIG_GET_ELOSD_LOGLEVEL_UTEST_H */
