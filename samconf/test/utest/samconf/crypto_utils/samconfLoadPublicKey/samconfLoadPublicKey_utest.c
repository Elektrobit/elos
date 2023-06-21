// SPDX-License-Identifier: MIT
#include "samconfLoadPublicKey_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfLoadPublicKeyUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfLoadPublicKeySuccess),
        TEST_CASE(samconfTestSamconfLoadPublicKeyNullPathError),
        TEST_CASE(samconfTestSamconfLoadPublicKeyFopenError),
        TEST_CASE(samconfTestSamconfLoadPublicKeyPEMReadError),
    };

    return RUN_TEST_SUITE(tests, samconfLoadPublicKeyUtest);
}

static int samconfLoadPublicKeyUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfLoadPublicKeyUtestTeardown(UNUSED void **state) {
    return 0;
}
