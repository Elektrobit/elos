// SPDX-License-Identifier: MIT
#include "elosPluginGetName_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosPluginGetNameUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginGetNameSuccess),
        TEST_CASE(elosTestElosPluginGetNameErrInvalidParameter),
    };

    return RUN_TEST_SUITE(tests, elosTestElosPluginGetNameUtest);
}

static int elosTestElosPluginGetNameUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosPluginGetNameUtestTeardown(UNUSED void **state) {
    return 0;
}
