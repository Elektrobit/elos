// SPDX-License-Identifier: MIT
#include "samconfConfigSetString_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfConfigSetStringUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(samconfTestSamconfConfigSetStringSuccess),
                                       TEST_CASE(samconfTestSamconfConfigSetStringErrorValueNull),
                                       TEST_CASE(samconfTestSamconfConfigSetStringErrorConfigNull),
                                       TEST_CASE(samconfTestSamconfConfigSetStringExtErrStrdup)};

    return RUN_TEST_SUITE(tests, samconfConfigSetStringUtest);
}

static int samconfConfigSetStringUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfConfigSetStringUtestTeardown(UNUSED void **state) {
    return 0;
}
