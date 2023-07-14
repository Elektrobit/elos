// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdLogLevel_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdLogLevelUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdLogLevelSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdLogLevelSuccessFromEnv),
        TEST_CASE(elosTestElosConfigGetElosdLogLevelSuccessFromConfig),
        TEST_CASE(elosTestElosConfigGetElosdLogLevelErrEnvFallbackToConfig),
        TEST_CASE(elosTestElosConfigGetElosdLogLevelErrFallbackToDefault),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdLogLevelUtest);
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

static int elosTestElosConfigGetElosdLogLevelUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdLogLevelUtestTeardown(UNUSED void **state) {
    return 0;
}
