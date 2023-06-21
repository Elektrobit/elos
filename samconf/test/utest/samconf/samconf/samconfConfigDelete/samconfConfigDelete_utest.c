// SPDX-License-Identifier: MIT
#include "samconfConfigDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigDeleteSuccess),
                                       TEST_CASE(samconfTestSamconfConfigDeleteNullConfiguration)};

    return RUN_TEST_SUITE(tests, samconfConfigDeleteUtest);
}

static int samconfConfigDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
