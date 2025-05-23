// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdLogFilter_utest.h"

int elosTestElosConfigGetElosdLogFilterSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdLogFilterSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdLogFilterSuccessFromDefault(UNUSED void **state) {
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdLogFilter");
    SHOULD("%s", "get the elos logFilter default option");

    const char *returnValue = elosConfigGetElosdLogFilter(&mockConfig);
    assert_string_equal(returnValue, ELOSD_LOG_FILTER);
}
