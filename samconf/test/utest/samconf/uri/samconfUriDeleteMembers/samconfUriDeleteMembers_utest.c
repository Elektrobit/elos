// SPDX-License-Identifier: MIT
#include "samconfUriDeleteMembers_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfUriDeleteMembersUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfUriDeleteMembersSuccess),
    };

    return RUN_TEST_SUITE(tests, samconfUriDeleteMembersUtest);
}

static int samconfUriDeleteMembersUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfUriDeleteMembersUtestTeardown(UNUSED void **state) {
    return 0;
}
