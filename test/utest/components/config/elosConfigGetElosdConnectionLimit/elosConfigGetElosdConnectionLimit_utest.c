// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdConnectionLimit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdConnectionLimitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitSuccessFromEnv),
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInEnv),
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitErrorInvalidLimitInDefault),
        TEST_CASE(elosTestElosConfigGetElosdConnectionLimitErrorLimitNotIntInEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdConnectionLimitUtest);
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

static int elosTestElosConfigGetElosdConnectionLimitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdConnectionLimitUtestTeardown(UNUSED void **state) {
    return 0;
}
