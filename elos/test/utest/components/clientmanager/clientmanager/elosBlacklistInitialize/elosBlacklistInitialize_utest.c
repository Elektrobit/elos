// SPDX-License-Identifier: MIT
#include "elosBlacklistInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosBlacklistInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosBlacklistInitializeSuccess),
        TEST_CASE(elosTestElosBlacklistInitializeExtErrFilterString),
        TEST_CASE(elosTestElosBlacklistInitializeExtErrConfigGetString),
        TEST_CASE(elosTestElosBlacklistInitializeErrBlacklistParameterNull),
        TEST_CASE(elosTestElosBlacklistInitializeErrConfigParameterNull),
        TEST_CASE(elosTestElosBlacklistInitializeErrFilterCreate),
    };
    return RUN_TEST_SUITE(tests, elosBlacklistInitializeUtest);
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

static int elosBlacklistInitializeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosBlacklistInitializeUtestTeardown(UNUSED void **state) {
    return 0;
}
