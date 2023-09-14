// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>

#include "elosClientManagerStart_utest.h"

int elosTestElosClientManagerStartExtErrInetPtonSetup(void **state) {
    elosClientManagerStartUtestInitParameters(state);
    return 0;
}

int elosTestElosClientManagerStartExtErrInetPtonTeardown(void **state) {
    elosClientManagerStartUtestDeleteParameters(state);
    return 0;
}

void elosTestElosClientManagerStartExtErrInetPton(void **state) {
    elosUteststateT_t *testState = *state;
    int retval = 0;

    TEST("elosClientManagerStart");
    SHOULD("%s", "return -1 when inet_pton fails");

    MOCK_FUNC_AFTER_CALL(inet_pton, 0);
    expect_value(__wrap_inet_pton, af, AF_INET);
    expect_any(__wrap_inet_pton, cp);
    expect_value(__wrap_inet_pton, buf, &(testState->context->addr.sin_addr));
    will_return(__wrap_inet_pton, -1);

    retval = elosClientManagerStart(testState->context, testState->parameters);

    assert_int_equal(retval, -1);
}
