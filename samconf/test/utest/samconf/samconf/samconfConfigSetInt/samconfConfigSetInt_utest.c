// SPDX-License-Identifier: MIT
#include "samconfConfigSetInt_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigSetIntUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigSetIntSuccess),
        TEST_CASE(samconfTestSamconfConfigSetIntErrorConfigNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigSetIntUtest);
}

static int samconfConfigSetIntUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigSetIntUtestTeardown(UNUSED void **state) {
    return 0;
}
