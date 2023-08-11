// SPDX-License-Identifier: MIT

#include <errno.h>

#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

int elosTestElosClientManagerThreadGetFreeConnectionSlotEintrSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    return 0;
}

int elosTestElosClientManagerThreadGetFreeConnectionSlotEintrTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadGetFreeConnectionSlotEintr(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    int slot = 0;

    TEST("elosClientManagerThreadGetFreeConnectionSlot");
    SHOULD("%s", "cover the errno = EINTR path successfully");

    errno = EINTR;

    MOCK_FUNC_AFTER_CALL(sem_timedwait, 0)
    expect_any(__wrap_sem_timedwait, __sem);
    expect_any(__wrap_sem_timedwait, __abstime);
    will_return(__wrap_sem_timedwait, -1);

    result = elosClientManagerThreadGetFreeConnectionSlot(context, &slot);
    assert_int_equal(result, SAFU_RESULT_OK);
}
