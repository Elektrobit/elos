// SPDX-License-Identifier: MIT
#include "samconfUriInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriInitSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfUriInitUtest);
}

static int samconfUriInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriInitUtestTeardown(UNUSED void **state) {
    return 0;
}
