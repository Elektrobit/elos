// SPDX-License-Identifier: MIT
#include "elosConfigGetElosdStorageBackendJsonFile_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigGetElosdStorageBackendJsonFileUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigGetElosdStorageBackendJsonFileSuccessFromDefault),
        TEST_CASE(elosTestElosConfigGetElosdStorageBackendJsonFileSuccessFromEnv),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigGetElosdStorageBackendJsonFileUtest);
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

static int elosTestElosConfigGetElosdStorageBackendJsonFileUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigGetElosdStorageBackendJsonFileUtestTeardown(UNUSED void **state) {
    return 0;
}
