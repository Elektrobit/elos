// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdBackendPath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdBackendPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdBackendPathSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdBackendPathSuccessFromEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdBackendPathUtest);
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

static int elosTestElosConfigGetElosdBackendPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdBackendPathUtestTeardown(UNUSED void **state) {
    return 0;
}
