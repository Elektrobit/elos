// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"
#include "mock_clientauthorization.h"

int elosTestElosClientManagerStartExtErrBindSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrBindTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationDelete, 0);
    expect_value(elosClientAuthorizationDelete, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationDelete, SAFU_RESULT_OK);

    elosClientManagerStop(testState->context);
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrBind(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when bind fails");

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationInitialize, 0);
    expect_value(elosClientAuthorizationInitialize, clientAuth, &(testState->context->clientAuth));
    will_return(elosClientAuthorizationInitialize, SAFU_RESULT_OK);

    MOCK_FUNC_AFTER_CALL(bind, 0);
    expect_any(__wrap_bind, fd);
    expect_value(__wrap_bind, addr, &(testState->context->addr));
    expect_value(__wrap_bind, len, sizeof(testState->context->addr));
    will_return(__wrap_bind, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}
