// SPDX-License-Identifier: MIT
#include "elosPluginControlGetName_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosPluginControlGetNameUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginControlGetNameSuccess),
        TEST_CASE(elosTestElosPluginControlGetNameErrInvalidParameter),
    };

    return RUN_TEST_SUITE(tests, elosTestElosPluginControlGetNameUtest);
}

static int elosTestElosPluginControlGetNameUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosPluginControlGetNameUtestTeardown(UNUSED void **state) {
    return 0;
}
