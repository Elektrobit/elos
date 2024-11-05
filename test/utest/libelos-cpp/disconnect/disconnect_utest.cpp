// SPDX-License-Identifier: MIT
#include "disconnect_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(disconnectUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestDisconnectSuccess),
    };

    return RUN_TEST_SUITE(tests, disconnectUtest);
}

static int disconnectUtestSetup(UNUSED void **state) {
    return 0;
}

static int disconnectUtestTeardown(UNUSED void **state) {
    return 0;
}
