// SPDX-License-Identifier: MIT
#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerThreadGetFreeConnectionSlotUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrClockGetTime),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrSemTimedwait),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexLock),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexUnlock01),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrMutexUnlock02),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotRecalcTvsec),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotEtimedout),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotEintr),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotThreadNotJoined),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotNoFreeConnection),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessFirst),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessMiddle),
        TEST_CASE(elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessLast)};
    return RUN_TEST_SUITE(tests, elosClientManagerThreadGetFreeConnectionSlotUtest);
}

static int elosClientManagerThreadGetFreeConnectionSlotUtestSetup(void **state) {
    elosClientManagerContext_t *context = safuAllocMem(NULL, sizeof(elosClientManagerContext_t));
    int retval = context == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    *state = context;
    return 0;
}

static int elosClientManagerThreadGetFreeConnectionSlotUtestTeardown(void **state) {
    elosClientManagerContext_t *context = *state;
    free(context);
    return 0;
}
