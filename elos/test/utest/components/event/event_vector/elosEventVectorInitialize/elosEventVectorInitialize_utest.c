// SPDX-License-Identifier: MIT

#include "elosEventVectorInitialize_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorInitializeUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventVectorInitializeErrEventVector),
        TEST_CASE(elosTestElosEventVectorInitializeSuccess),
    };

    return RUN_TEST_SUITE(tests, elosEventVectorInitializeUtest);
}

static int elosEventVectorInitializeUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorInitializeUtestTeardown(UNUSED void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
