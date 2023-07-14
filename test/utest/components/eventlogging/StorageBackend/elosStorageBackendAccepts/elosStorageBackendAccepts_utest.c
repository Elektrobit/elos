// SPDX-License-Identifier: MIT
#include "elosStorageBackendAccepts_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosStorageBackendAcceptsUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosStorageBackendAcceptsErrInvalidParameter),
        TEST_CASE(elosTestElosStorageBackendAcceptsSuccess),
    };
    return RUN_TEST_SUITE(tests, elosStorageBackendAcceptsUtest);
}

static int elosStorageBackendAcceptsUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosStorageBackendAcceptsUtestTeardown(UNUSED void **state) {
    return 0;
}
