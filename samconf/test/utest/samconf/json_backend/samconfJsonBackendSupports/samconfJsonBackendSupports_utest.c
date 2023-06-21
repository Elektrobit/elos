// SPDX-License-Identifier: MIT
#include "samconfJsonBackendSupports_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfJsonBackendSupportsUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfJsonBackendSupportsFileSuccess),
                                       TEST_CASE(samconfTestSamconfJsonBackendSupportsNoInvalidExtension)};

    return RUN_TEST_SUITE(tests, samconfJsonBackendSupportsUtest);
}

static int samconfJsonBackendSupportsUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfJsonBackendSupportsUtestTeardown(UNUSED void **state) {
    return 0;
}
