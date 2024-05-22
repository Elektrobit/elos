// SPDX-License-Identifier: MIT
#ifndef ELOS_CONFIG_GET_ELOSD_INTERFACE_UTEST_H
#define ELOS_CONFIG_GET_ELOSD_INTERFACE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/mock_safu.h>
#include <samconf/mock_samconf.h>

samconfConfig_t elosGetMockConfig();

TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdInterfaceSuccessFromEnv)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdInterfaceSuccessFromDefault)

#endif /* ELOS_CONFIG_GET_ELOSD_INTERFACE_UTEST_H */
