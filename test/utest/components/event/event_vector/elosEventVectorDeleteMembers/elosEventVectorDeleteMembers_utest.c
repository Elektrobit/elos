// SPDX-License-Identifier: MIT

#include "elosEventVectorDeleteMembers_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorDeleteMembersUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorDeleteMembersErrEventVector),
                                       TEST_CASE(elosTestElosEventVectorDeleteMembersSuccess)};

    return RUN_TEST_SUITE(tests, elosEventVectorDeleteMembersUtest);
}

static int elosEventVectorDeleteMembersUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorDeleteMembersUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
