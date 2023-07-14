// SPDX-License-Identifier: MIT
#include "elosJsonBackendPersist_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosJsonBackendPersistUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosJsonBackendPersistExterrEventSerialize),
        TEST_CASE(elosTestElosJsonBackendPersistExterrWriteExactly),
        TEST_CASE(elosTestElosJsonBackendPersistSuccess),
    };
    return RUN_TEST_SUITE(tests, elosJsonBackendPersistUtest);
}

static int elosJsonBackendPersistUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosJsonBackendPersistUtestTeardown(UNUSED void **state) {
    return 0;
}
