// SPDX-License-Identifier: MIT
#include "samconfConfigBackendDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigBackendDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigBackendDeleteSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfConfigBackendDeleteUtest);
}

static int samconfConfigBackendDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigBackendDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
