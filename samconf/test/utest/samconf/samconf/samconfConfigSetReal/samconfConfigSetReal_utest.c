// SPDX-License-Identifier: MIT
#include "samconfConfigSetReal_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigSetRealUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigSetRealSuccess),
        TEST_CASE(samconfTestSamconfConfigSetRealErrorConfigNull),
    };

    return RUN_TEST_SUITE(tests, samconfConfigSetRealUtest);
}

static int samconfConfigSetRealUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigSetRealUtestTeardown(UNUSED void **state) {
    return 0;
}
