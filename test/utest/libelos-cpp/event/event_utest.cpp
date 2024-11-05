// SPDX-License-Identifier: MIT
#include "event_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(eventUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEventSuccess),
    };

    return RUN_TEST_SUITE(tests, eventUtest);
}

static int eventUtestSetup(UNUSED void **state) {
    return 0;
}

static int eventUtestTeardown(UNUSED void **state) {
    return 0;
}
