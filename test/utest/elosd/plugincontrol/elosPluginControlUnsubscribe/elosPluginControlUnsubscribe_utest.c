// SPDX-License-Identifier: MIT
#include "elosPluginControlUnsubscribe_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosTestElosPluginControlUnsubscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginControlUnsubscribeSuccess),
        TEST_CASE(elosTestElosPluginControlUnsubscribeErrInvalidParameter),
    };

    return RUN_TEST_SUITE(tests, elosTestElosPluginControlUnsubscribeUtest);
}

static int elosTestElosPluginControlUnsubscribeUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosTestElosPluginControlUnsubscribeUtestTeardown(UNUSED void **state) {
    return 0;
}
