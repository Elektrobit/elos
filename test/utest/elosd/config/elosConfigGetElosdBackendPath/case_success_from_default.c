// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdBackendPath_utest.h"

int elosTestElosConfigGetElosdBackendPathSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdBackendPathSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdBackendPathSuccessFromDefault(UNUSED void **state) {
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdBackendPath");
    SHOULD("%s", "get the elos backend path default option");

    const char *returnValue = elosConfigGetElosdBackendPath(&mockConfig);
    assert_string_equal(returnValue, ELOSD_BACKEND_PATH);
}
