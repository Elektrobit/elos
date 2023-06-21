// SPDX-License-Identifier: MIT
#include "samconfUriDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriDeleteSuccess),
        TEST_CASE(samconfTestSamconfUriDeleteMembersError),
    };

    return RUN_TEST_SUITE(tests, samconfUriDeleteUtest);
}

static int samconfUriDeleteUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriDeleteUtestTeardown(UNUSED void **state) {
    return 0;
}
