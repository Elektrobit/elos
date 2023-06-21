// SPDX-License-Identifier: MIT
#include "samconfConfigInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigInitUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigInitSuccess),
                                       TEST_CASE(samconfTestSamconfConfigInitErrorNullConfig)};

    return RUN_TEST_SUITE(tests, samconfConfigInitUtest);
}

static int samconfConfigInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigInitUtestTeardown(UNUSED void **state) {
    return 0;
}
