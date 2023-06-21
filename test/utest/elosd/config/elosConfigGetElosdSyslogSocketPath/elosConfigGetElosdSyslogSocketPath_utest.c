// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdSyslogSocketPath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdSyslogSocketPathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdSyslogSocketPathSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdSyslogSocketPathSuccessFromEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdSyslogSocketPathUtest);
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

static int elosTestElosConfigGetElosdSyslogSocketPathUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdSyslogSocketPathUtestTeardown(UNUSED void **state) {
    return 0;
}
