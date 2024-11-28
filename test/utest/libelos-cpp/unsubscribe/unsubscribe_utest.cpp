// SPDX-License-Identifier: MIT
#include "unsubscribe_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(unsubscribeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestUnsubscribeSuccess),
    };

    return RUN_TEST_SUITE(tests, unsubscribeUtest);
}

static int unsubscribeUtestSetup(UNUSED void **state) {
    return 0;
}

static int unsubscribeUtestTeardown(UNUSED void **state) {
    return 0;
}
