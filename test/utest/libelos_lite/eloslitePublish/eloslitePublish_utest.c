// SPDX-License-Identifier: MIT

#include "eloslitePublish_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(publishUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosliteTestEloslitePublishSuccessAllPermutations),
    };

    return RUN_TEST_SUITE(tests, publishUtest);
}

static int publishUtestSetup(UNUSED void **state) {
    return 0;
}

static int publishUtestTeardown(UNUSED void **state) {
    return 0;
}
