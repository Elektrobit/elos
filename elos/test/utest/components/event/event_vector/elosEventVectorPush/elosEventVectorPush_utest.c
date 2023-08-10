// SPDX-License-Identifier: MIT

#include "elosEventVectorPush_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorPushUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorPushMock)};

    return RUN_TEST_SUITE(tests, elosEventVectorPushUtest);
}

static int elosEventVectorPushUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosEventVectorPushUtestTeardown(UNUSED void **state) {
    return 0;
}
