// SPDX-License-Identifier: MIT

#include "elosEventVectorNew_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorNewUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventVectorNewErrEventVector),
        TEST_CASE(elosTestElosEventVectorExtErrAllocMem),
        TEST_CASE(elosTestElosEventVectorNewSuccess),
    };

    return RUN_TEST_SUITE(tests, elosEventVectorNewUtest);
}

static int elosEventVectorNewUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorNewUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
