// SPDX-License-Identifier: MIT
#include "samconfUriNew_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriNewUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriNewSuccess),
        TEST_CASE(samconfTestSamconfUriNewMemError),
        TEST_CASE(samconfTestSamconfUriNewInitError),
    };

    return RUN_TEST_SUITE(tests, samconfUriNewUtest);
}

static int samconfUriNewUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriNewUtestTeardown(UNUSED void **state) {
    return 0;
}
