// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadWaitForIncomingConnection_utest.h"

int elosTestElosClientManagerThreadWaitForIncomingConnectionErrPselectSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    return 0;
}

int elosTestElosClientManagerThreadWaitForIncomingConnectionErrPselectTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadWaitForIncomingConnectionErrPselect(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    int slot = 0;

    TEST("elosClientManagerThreadWaitForIncomingConnection");
    SHOULD("%s", "return SAFU_RESULT_FAILED when pselect fails");

    MOCK_FUNC_AFTER_CALL(elosClientManagerGetStatus, 0);
    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(pselect, 0);
    expect_value(__wrap_pselect, nfds, context->fd + 1);
    expect_not_value(__wrap_pselect, readfds, NULL);
    expect_value(__wrap_pselect, writefds, NULL);
    expect_value(__wrap_pselect, exceptfds, NULL);
    expect_not_value(__wrap_pselect, timeout, NULL);
    expect_value(__wrap_pselect, sigmask, NULL);
    will_set_errno(__wrap_pselect, EINVAL);
    will_return(__wrap_pselect, -1);

    result = elosClientManagerThreadWaitForIncomingConnection(context, slot);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
