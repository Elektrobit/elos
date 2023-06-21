// SPDX-License-Identifier: MIT
#include "elosClientManagerThreadListen_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerThreadListenUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosClientManagerThreadListenErrGetStatus),
                                       TEST_CASE(elosTestElosClientManagerThreadListenInactive),
                                       TEST_CASE(elosTestElosClientManagerThreadErrGetFreeSlot),
                                       TEST_CASE(elosTestElosClientManagerThreadSlotLtZero),
                                       TEST_CASE(elosTestElosClientManagerThreadErrMutexLock),
                                       TEST_CASE(elosTestElosClientManagerThreadErrWaitForConnection),
                                       TEST_CASE(elosTestElosClientManagerThreadErrMutexUnlock01),
                                       TEST_CASE(elosTestElosClientManagerThreadErrSemUnlock01),
                                       TEST_CASE(elosTestElosClientManagerThreadErrPthreadCreate),
                                       TEST_CASE(elosTestElosClientManagerThreadErrSemUnlock02),
                                       TEST_CASE(elosTestElosClientManagerThreadErrMutexUnlock02),
                                       TEST_CASE(elosTestElosClientManagerThreadSuccessOnes),
                                       TEST_CASE(elosTestElosClientManagerThreadSuccessAll)};
    return RUN_TEST_SUITE(tests, elosClientManagerThreadListenUtest);
}

static int elosClientManagerThreadListenUtestSetup(void **state) {
    elosClientManagerContext_t *context = safuAllocMem(NULL, sizeof(elosClientManagerContext_t));
    int retval = context == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    *state = context;
    return 0;
}

static int elosClientManagerThreadListenUtestTeardown(void **state) {
    elosClientManagerContext_t *context = *state;
    free(context);
    return 0;
}
