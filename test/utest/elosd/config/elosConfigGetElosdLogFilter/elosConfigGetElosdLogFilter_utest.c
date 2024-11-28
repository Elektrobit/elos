// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdLogFilter_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdLogFilterUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdLogFilterSuccessFromEnv),
        TEST_CASE(elosTestElosConfigGetElosdLogFilterSuccessFromDefault),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdLogFilterUtest);
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

static int elosTestElosConfigGetElosdLogFilterUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdLogFilterUtestTeardown(UNUSED void **state) {
    return 0;
}
