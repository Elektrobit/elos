// SPDX-License-Identifier: MIT
#include "elosClientManagerThreadWaitForIncomingConnection_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerThreadWaitForIncomingConnectionUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionErrGetStatus),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionListenInactive),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionErrPselect),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionErrAccept),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionInterrupt),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionSuccessFdReady),
        TEST_CASE(elosTestElosClientManagerThreadWaitForIncomingConnectionSuccessFdZero)};
    return RUN_TEST_SUITE(tests, elosClientManagerThreadWaitForIncomingConnectionUtest);
}

static int elosClientManagerThreadWaitForIncomingConnectionUtestSetup(void **state) {
    elosClientManagerContext_t *context = safuAllocMem(NULL, sizeof(elosClientManagerContext_t));
    int retval = context == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    *state = context;
    return 0;
}

static int elosClientManagerThreadWaitForIncomingConnectionUtestTeardown(void **state) {
    elosClientManagerContext_t *context = *state;
    free(context);
    return 0;
}
