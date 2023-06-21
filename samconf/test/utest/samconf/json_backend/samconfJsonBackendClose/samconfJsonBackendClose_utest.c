// SPDX-License-Identifier: MIT
#include "samconfJsonBackendClose_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfJsonBackendCloseUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfJsonBackendCloseSuccess),
        TEST_CASE(samconfTestSamconfJsonBackendCloseFailure),
    };

    return RUN_TEST_SUITE(tests, samconfJsonBackendCloseUtest);
}

static int samconfJsonBackendCloseUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfJsonBackendCloseUtestTeardown(UNUSED void **state) {
    return 0;
}
