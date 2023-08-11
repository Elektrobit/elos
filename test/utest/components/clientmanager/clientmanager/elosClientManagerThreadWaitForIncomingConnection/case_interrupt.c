// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadWaitForIncomingConnection_utest.h"
#include "mock_clientauthorization.h"

int elosTestElosClientManagerThreadWaitForIncomingConnectionInterruptSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    return 0;
}

int elosTestElosClientManagerThreadWaitForIncomingConnectionInterruptTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadWaitForIncomingConnectionInterrupt(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    int slot = 0;

    TEST("elosClientManagerThreadWaitForIncomingConnection");
    SHOULD("%s", "cover the errno = EINTR path successfully");

    MOCK_FUNC_ALWAYS(elosClientManagerGetStatus);
    MOCK_FUNC_ALWAYS(pselect);
    MOCK_FUNC_ALWAYS(accept);

    PARAM("%s", "pselect path");

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    expect_value(__wrap_pselect, nfds, context->fd + 1);
    expect_not_value(__wrap_pselect, readfds, NULL);
    expect_value(__wrap_pselect, writefds, NULL);
    expect_value(__wrap_pselect, exceptfds, NULL);
    expect_not_value(__wrap_pselect, timeout, NULL);
    expect_value(__wrap_pselect, sigmask, NULL);
    will_set_errno(__wrap_pselect, EINTR);
    will_return(__wrap_pselect, -1);

    PARAM("%s", "accept path");

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    expect_value(__wrap_pselect, nfds, context->fd + 1);
    expect_not_value(__wrap_pselect, readfds, NULL);
    expect_value(__wrap_pselect, writefds, NULL);
    expect_value(__wrap_pselect, exceptfds, NULL);
    expect_not_value(__wrap_pselect, timeout, NULL);
    expect_value(__wrap_pselect, sigmask, NULL);
    will_set_errno(__wrap_pselect, EINTR);
    will_return(__wrap_pselect, 1);

    expect_value(__wrap_accept, fd, context->fd);
    expect_value(__wrap_accept, addr, (struct sockaddr *)&context->connection[slot].addr);
    expect_not_value(__wrap_accept, len, NULL);
    will_set_errno(__wrap_accept, EINTR);
    will_return(__wrap_accept, -1);

    PARAM("%s", "finish test");

    expect_value(elosClientManagerGetStatus, context, context);
    expect_not_value(elosClientManagerGetStatus, status, NULL);
    will_set_parameter(elosClientManagerGetStatus, status, context->status);
    will_return(elosClientManagerGetStatus, SAFU_RESULT_OK);

    expect_value(__wrap_pselect, nfds, context->fd + 1);
    expect_not_value(__wrap_pselect, readfds, NULL);
    expect_value(__wrap_pselect, writefds, NULL);
    expect_value(__wrap_pselect, exceptfds, NULL);
    expect_not_value(__wrap_pselect, timeout, NULL);
    expect_value(__wrap_pselect, sigmask, NULL);
    will_set_errno(__wrap_pselect, EINTR);
    will_return(__wrap_pselect, 1);

    expect_value(__wrap_accept, fd, context->fd);
    expect_value(__wrap_accept, addr, (struct sockaddr *)&context->connection[slot].addr);
    expect_not_value(__wrap_accept, len, NULL);
    will_set_errno(__wrap_accept, EINTR);
    will_return(__wrap_accept, 1);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationIsTrustedConnection, 0);
    expect_value(elosClientAuthorizationIsTrustedConnection, clientAuth, &(context->clientAuth));
    expect_value(elosClientAuthorizationIsTrustedConnection, addr, &(context->connection[slot].addr));
    will_return(elosClientAuthorizationIsTrustedConnection, SAFU_RESULT_OK);

    result = elosClientManagerThreadWaitForIncomingConnection(context, slot);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(accept);
    MOCK_FUNC_NEVER(pselect);
    MOCK_FUNC_NEVER(elosClientManagerGetStatus);
}
