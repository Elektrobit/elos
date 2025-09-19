// SPDX-License-Identifier: MIT

#include "elosDltMapperInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosDltMapperInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestelosDltMapperInitSuccess),
        TEST_CASE(elosTestelosDltMapperInitError),
    };

    return RUN_TEST_SUITE(tests, elosDltMapperInitUtest);
}

static int elosDltMapperInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosDltMapperInitUtestTeardown(UNUSED void **state) {
    return 0;
}
