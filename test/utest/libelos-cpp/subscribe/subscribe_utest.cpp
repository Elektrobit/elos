// SPDX-License-Identifier: MIT
#include "subscribe_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(subscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestSubscribeSuccess),
    };

    return RUN_TEST_SUITE(tests, subscribeUtest);
}

static int subscribeUtestSetup(UNUSED void **state) {
    return 0;
}

static int subscribeUtestTeardown(UNUSED void **state) {
    return 0;
}
