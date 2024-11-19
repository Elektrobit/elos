// SPDX-License-Identifier: MIT
#include "read_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(readUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestReadSuccess),
        TEST_CASE(elosTestReadError),
    };

    return RUN_TEST_SUITE(tests, readUtest);
}

static int readUtestSetup(UNUSED void **state) {
    return 0;
}

static int readUtestTeardown(UNUSED void **state) {
    return 0;
}
