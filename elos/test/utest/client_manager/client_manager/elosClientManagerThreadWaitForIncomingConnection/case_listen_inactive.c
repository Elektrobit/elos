// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadWaitForIncomingConnection_utest.h"

int elosTestElosClientManagerThreadWaitForIncomingConnectionListenInactiveSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    return 0;
}

int elosTestElosClientManagerThreadWaitForIncomingConnectionListenInactiveTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadWaitForIncomingConnectionListenInactive(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_OK;
    int slot = 0;

    TEST("elosClientManagerThreadWaitForIncomingConnection");
    SHOULD("%s", "return SAFU_RESULT_OK when thread stopped listening");

    MOCK_FUNC_AFTER_CALL(elosClientManagerGetStatus, 0);
    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, 0);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    result = elosClientManagerThreadWaitForIncomingConnection(context, slot);
    assert_int_equal(result, SAFU_RESULT_OK);
}
