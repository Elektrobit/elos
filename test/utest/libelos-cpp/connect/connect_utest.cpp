// SPDX-License-Identifier: MIT
#include "connect_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(connectUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestConnectSuccess),
    };

    return RUN_TEST_SUITE(tests, connectUtest);
}

static int connectUtestSetup(UNUSED void **state) {
    return 0;
}

static int connectUtestTeardown(UNUSED void **state) {
    return 0;
}
