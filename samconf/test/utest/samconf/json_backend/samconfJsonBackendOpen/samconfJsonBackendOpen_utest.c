// SPDX-License-Identifier: MIT
#include "samconfJsonBackendOpen_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfJsonBackendOpenUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfJsonBackendOpenFileSuccess),
        TEST_CASE(samconfTestSamconfJsonBackendOpenFileError),
    };

    return RUN_TEST_SUITE(tests, samconfJsonBackendOpenUtest);
}

static int samconfJsonBackendOpenUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfJsonBackendOpenUtestTeardown(UNUSED void **state) {
    return 0;
}
