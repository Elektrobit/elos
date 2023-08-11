// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadGetFreeConnectionSlot_utest.h"

int elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessFirstSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));

    context->connection[0].status |= CLIENT_MANAGER_CONNECTION_ACTIVE;
    return 0;
}

int elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessFirstTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadGetFreeConnectionSlotSuccessFirst(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    int slot = 0;

    TEST("elosClientManagerThreadGetFreeConnectionSlot");
    SHOULD("%s", "successfully get a connection slot");

    MOCK_FUNC_AFTER_CALL(sem_timedwait, 0)
    expect_any(__wrap_sem_timedwait, __sem);
    expect_any(__wrap_sem_timedwait, __abstime);
    will_return(__wrap_sem_timedwait, 0);

    result = elosClientManagerThreadGetFreeConnectionSlot(context, &slot);
    assert_int_equal(result, SAFU_RESULT_OK);
}
