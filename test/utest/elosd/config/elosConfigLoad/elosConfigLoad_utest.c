// SPDX-License-Identifier: MIT
#include "elosConfigLoad_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosConfigLoadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosConfigLoadDefaultConfigSuccess),   TEST_CASE(elosTestElosConfigLoadDefaultDirSuccess),
        TEST_CASE(elosTestElosConfigLoadInvalidConfigPathError), TEST_CASE(elosTestElosConfigLoadMultipleConfigsError),
        TEST_CASE(elosTestElosConfigLoadEnvVariableConfigError),
    };

    return RUN_TEST_SUITE(tests, elosTestElosConfigLoadUtest);
}

static int elosTestElosConfigLoadUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosConfigLoadUtestTeardown(UNUSED void **state) {
    return 0;
}
