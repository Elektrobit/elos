// SPDX-License-Identifier: MIT
#include "samconfSignatureLoaderGetSignatureFor_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfSignatureLoaderGetSignatureForUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfSignatureLoaderGetSignatureForSuccess),
        TEST_CASE(samconfTestSamconfSignatureLoaderGetSignatureForAllocError),
        TEST_CASE(samconfTestSamconfSignatureLoaderGetSignatureForAccessError),
        TEST_CASE(samconfTestSamconfSignatureLoaderGetSignatureForReadError),
    };

    return RUN_TEST_SUITE(tests, samconfSignatureLoaderGetSignatureForUtest);
}

static int samconfSignatureLoaderGetSignatureForUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfSignatureLoaderGetSignatureForUtestTeardown(UNUSED void **state) {
    return 0;
}
