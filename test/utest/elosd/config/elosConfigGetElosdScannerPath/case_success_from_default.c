// SPDX-License-Identifier: MIT

#include "elosConfigGetElosdScannerPath_utest.h"

int elosTestElosConfigGetElosdScannerPathSuccessFromDefaultSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConfigGetElosdScannerPathSuccessFromDefaultTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConfigGetElosdScannerPathSuccessFromDefault(UNUSED void **state) {
    samconfConfig_t mockConfig = elosGetMockConfig();

    TEST("elosConfigGetElosdScannerPath");
    SHOULD("%s", "get the elos scanner path default option");

    const char *returnValue = elosConfigGetElosdScannerPath(&mockConfig);
    assert_string_equal(returnValue, ELOSD_SCANNER_PATH);
}
