// SPDX-License-Identifier: MIT
#include "elosCompleteStoragePath_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosCompleteStoragePathUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(ELOS_testelosCompleteStoragePathSuccess),
    };
    return RUN_TEST_SUITE(tests, elosCompleteStoragePathUtest);
}

static int elosCompleteStoragePathUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosCompleteStoragePathUtestTeardown(UNUSED void **state) {
    return 0;
}
