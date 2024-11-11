// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/libelos/libelos.h"
#include "elosDisconnect_utest.h"

int elosTestElosDisconnectExtErrCloseSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    test->session = safuAllocMem(NULL, sizeof(elosSession_t));
    assert_non_null(test);

    memset(test->session, 0x00, sizeof(elosSession_t));

    test->session->connected = true;
    test->session->fd = MOCK_FD;

    return 0;
}

int elosTestElosDisconnectExtErrCloseTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->session);
    test->session = NULL;

    return 0;
}

void elosTestElosDisconnectExtErrClose(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosDisconnect");
    SHOULD("%s", "return -1 if close fails");

    MOCK_FUNC_AFTER_CALL(elosDisconnectSession, 0);
    expect_any(elosDisconnectSession, session);
    will_return(elosDisconnectSession, SAFU_RESULT_FAILED);

    result = elosDisconnect(test->session);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    test->session = NULL;
}
