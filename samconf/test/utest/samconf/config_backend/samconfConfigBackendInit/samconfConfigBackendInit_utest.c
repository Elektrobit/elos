// SPDX-License-Identifier: MIT
#include "samconfConfigBackendInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigBackendInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigBackendInitSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfConfigBackendInitUtest);
}

static int samconfConfigBackendInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigBackendInitUtestTeardown(UNUSED void **state) {
    return 0;
}
