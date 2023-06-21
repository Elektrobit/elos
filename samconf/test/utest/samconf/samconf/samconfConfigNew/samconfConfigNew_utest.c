// SPDX-License-Identifier: MIT
#include "samconfConfigNew_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigNewUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigNewSuccess),
        TEST_CASE(samconfTestSamconfConfigNewAllocMemError),
        TEST_CASE(samconfTestSamconfConfigNewErrorNullConfig),
    };

    return RUN_TEST_SUITE(tests, samconfConfigNewUtest);
}

static int samconfConfigNewUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigNewUtestTeardown(UNUSED void **state) {
    return 0;
}
