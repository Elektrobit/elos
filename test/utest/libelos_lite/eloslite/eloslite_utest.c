// SPDX-License-Identifier: MIT

#include "eloslite_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(publishUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosliteTestElosliteLogMessageFormatedSuccess),
    };

    return RUN_TEST_SUITE(tests, publishUtest);
}

static int publishUtestSetup(UNUSED void **state) {
    return 0;
}

static int publishUtestTeardown(UNUSED void **state) {
    return 0;
}
