// SPDX-License-Identifier: MIT

#include "elosClientManagerThreadWaitForIncomingConnection_utest.h"
#include "mock_client_authorization.h"

#define MOCK_FD_CONTAINED_IN_SET 3
#define MOCK_SOCKET_FD           42

int elosTestElosClientManagerThreadWaitForIncomingConnectionSuccessFdZeroSetup(void **state) {
    elosClientManagerContext_t *context = *state;
    memset(context, '\0', sizeof(elosClientManagerContext_t));
    context->status |= CLIENT_MANAGER_LISTEN_ACTIVE;
    return 0;
}

int elosTestElosClientManagerThreadWaitForIncomingConnectionSuccessFdZeroTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosClientManagerThreadWaitForIncomingConnectionSuccessFdZero(void **state) {
    elosClientManagerContext_t *context = *state;
    safuResultE_t result = SAFU_RESULT_FAILED;
    int slot = 0;

    TEST("elosClientManagerThreadWaitForIncomingConnection");
    SHOULD("%s", "successfully create a connection when pselect timeout expires ones");

    MOCK_FUNC_ALWAYS(elosClientManagerGetStatus);
    MOCK_FUNC_ALWAYS(pselect);

    PARAM("%s", "simulate pselect returns 0");

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
    will_return(__wrap_pselect, 0);

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
    will_return(__wrap_pselect, MOCK_FD_CONTAINED_IN_SET);

    MOCK_FUNC_AFTER_CALL(accept, 0);
    expect_value(__wrap_accept, fd, context->fd);
    expect_value(__wrap_accept, addr, (struct sockaddr *)&context->connection[slot].addr);
    expect_not_value(__wrap_accept, len, NULL);
    will_return(__wrap_accept, MOCK_SOCKET_FD);

    MOCK_FUNC_AFTER_CALL(elosClientAuthorizationIsTrustedConnection, 0);
    expect_value(elosClientAuthorizationIsTrustedConnection, clientAuth, &(context->clientAuth));
    expect_value(elosClientAuthorizationIsTrustedConnection, addr, &(context->connection[slot].addr));
    will_return(elosClientAuthorizationIsTrustedConnection, SAFU_RESULT_OK);

    result = elosClientManagerThreadWaitForIncomingConnection(context, slot);
    assert_int_equal(result, SAFU_RESULT_OK);

    MOCK_FUNC_NEVER(pselect);
    MOCK_FUNC_NEVER(elosClientManagerGetStatus);
}
