// SPDX-License-Identifier: MIT
#include "samconfUriPattern_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriPatternUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriPatternRegCompError),
        TEST_CASE(samconfTestSamconfUriPatternSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfUriPatternUtest);
}

static int samconfUriPatternUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriPatternUtestTeardown(UNUSED void **state) {
    return 0;
}
