// SPDX-License-Identifier: MIT

#include <cmocka_extensions/mock_extensions.h>
#include <safu/log.h>

#include "cmocka_mocks/mock_libc.h"
#include "elosConfigGetElosdLogLevel_utest.h"

int elosTestElosConfigGetElosdLogLevelSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdLogLevelSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdLogLevelSuccessFromDefault(UNUSED void **state) {
    safuLogLevelE_t expectedValue = safuLogLevelFromString(ELOSD_LOG_LEVEL);
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdLogLevel");
    SHOULD("%s", "get the elos LogLevel default option");

    safuLogLevelE_t returnValue = elosConfigGetElosdLogLevel(&mockConfig);
    assert_int_equal(returnValue, expectedValue);
}
