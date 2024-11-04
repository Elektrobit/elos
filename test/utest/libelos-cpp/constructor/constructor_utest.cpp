// SPDX-License-Identifier: MIT
#include "constructor_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(constructorUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestConstructorSuccess),
    };

    return RUN_TEST_SUITE(tests, constructorUtest);
}

static int constructorUtestSetup(UNUSED void **state) {
    return 0;
}

static int constructorUtestTeardown(UNUSED void **state) {
    return 0;
}
