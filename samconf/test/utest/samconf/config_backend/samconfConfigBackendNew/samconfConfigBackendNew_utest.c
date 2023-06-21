// SPDX-License-Identifier: MIT
#include "samconfConfigBackendNew_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigBackendNewUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfConfigBackendNewSuccess),
        TEST_CASE(samconfTestSamconfConfigBackendNewMemError),
        TEST_CASE(samconfTestSamconfConfigBackendNewInitError),
    };

    return RUN_TEST_SUITE(tests, samconfConfigBackendNewUtest);
}

static int samconfConfigBackendNewUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigBackendNewUtestTeardown(UNUSED void **state) {
    return 0;
}
