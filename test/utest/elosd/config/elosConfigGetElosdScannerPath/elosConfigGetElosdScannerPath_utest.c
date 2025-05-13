// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdScannerPath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdScannerPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdScannerPathSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdScannerPathSuccessFromConfig),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdScannerPathUtest);
}

samconfConfig_t elosGetMockConfig() {
    samconfConfig_t configData = {
        .parent = NULL,
        .key = "mockConfig",
        .type = SAMCONF_CONFIG_VALUE_INT,
        .value.integer = 42,
        .children = NULL,
        .childCount = 0,
    };
    return configData;
}

static int elosTestElosConfigGetElosdScannerPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdScannerPathUtestTeardown(UNUSED void **state) {
    return 0;
}
