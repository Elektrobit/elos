// SPDX-License-Identifier: MIT
#include "samconfCryptoUtilsVerify_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfCryptoUtilsVerifyUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfCryptoUtilsVerifyErrorMismatch),
        TEST_CASE(samconfTestSamconfCryptoUtilsVerifyErrorNull),
        TEST_CASE(samconfTestSamconfCryptoUtilsVerifySuccess),
    };

    return RUN_TEST_SUITE(tests, samconfCryptoUtilsVerifyUtest);
}

static int samconfCryptoUtilsVerifyUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfCryptoUtilsVerifyUtestTeardown(UNUSED void **state) {
    return 0;
}
