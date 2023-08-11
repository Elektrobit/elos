// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

int elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrClockGetTimeSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    return 0;
}

int elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrClockGetTimeTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadGetFreeConnectionSlotExtErrClockGetTime(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    struct timespec ts = {.tv_nsec = TIMESPEC_1SEC_IN_NSEC, .tv_sec = 0x43};
    int slot = 0;

    TEST("elosClientManagerThreadGetFreeConnectionSlot");
    SHOULD("%s", "return SAFU_RESULT_FAILED when clock_gettime failed");

    MOCK_FUNC_AFTER_CALL(clock_gettime, 0)
    expect_any(__wrap_clock_gettime, clock_id);
    expect_any(__wrap_clock_gettime, tp);
    will_set_parameter(__wrap_clock_gettime, tp, &ts);
    will_return(__wrap_clock_gettime, -1);

    MOCK_FUNC_AFTER_CALL(raise, 0)
    expect_value(__wrap_raise, __sig, SIGTERM);
    will_return(__wrap_raise, 0);

    result = elosClientManagerThreadGetFreeConnectionSlot(context, &slot);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
