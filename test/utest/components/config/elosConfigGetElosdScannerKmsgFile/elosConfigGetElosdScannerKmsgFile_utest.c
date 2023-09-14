// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdScannerKmsgFile_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdScannerKmsgFileUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdScannerKmsgFileSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdScannerKmsgFileSuccessFromEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdScannerKmsgFileUtest);
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

static int elosTestElosConfigGetElosdScannerKmsgFileUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdScannerKmsgFileUtestTeardown(UNUSED void **state) {
    return 0;
}
