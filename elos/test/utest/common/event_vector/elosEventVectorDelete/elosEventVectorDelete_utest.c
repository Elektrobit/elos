// SPDX-License-Identifier: MIT

#include "elosEventVectorDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosEventVectorDeleteUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosEventVectorDeleteErrEventVector),
                                       TEST_CASE(elosTestElosEventVectorDeleteSuccess)};

    return RUN_TEST_SUITE(tests, elosEventVectorDeleteUtest);
}

static int elosEventVectorDeleteUtestSetup(UNUSED void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    assert_non_null(testState);
    *state = testState;
    return 0;
}

static int elosEventVectorDeleteUtestTeardown(UNUSED void **state) {
    elosUteststateT_t *testState = *state;
    free(testState);
    return 0;
}
