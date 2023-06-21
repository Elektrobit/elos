// SPDX-License-Identifier: MIT
#include "samconfConfigSetBool_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigSetBoolUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigSetBoolSuccess),
        TEST_CASE(samconfTestSamconfConfigSetBoolErrorConfigNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigSetBoolUtest);
}

static int samconfConfigSetBoolUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigSetBoolUtestTeardown(UNUSED void **state) {
    return 0;
}
