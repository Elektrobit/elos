// SPDX-License-Identifier: MIT
#include "samconfBase64Decode_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfBase64DecodeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfBase64DecodeSuccess),
        TEST_CASE(samconfTestSamconfBase64DecodeAllocError),
        TEST_CASE(samconfTestSamconfBase64DecodeDecodeError),
        TEST_CASE(samconfTestSamconfBase64DecodePaddingError),
    };

    return RUN_TEST_SUITE(tests, samconfBase64DecodeUtest);
}

static int samconfBase64DecodeUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfBase64DecodeUtestTeardown(UNUSED void **state) {
    return 0;
}
