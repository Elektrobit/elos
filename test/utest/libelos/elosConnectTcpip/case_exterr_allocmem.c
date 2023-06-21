// SPDX-License-Identifier: MIT

#include "elos/libelos/libelos.h"
#include "elosConnectTcpip_utest.h"
#include "safu/mock_safu.h"

int elosTestElosConnectTcpipExtErrAllocMemSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosConnectTcpipExtErrAllocMemTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosConnectTcpipExtErrAllocMem(UNUSED void **state) {
    elosSession_t *newSession = NULL;
    safuResultE_t result;

    TEST("elosConnectTcpip");
    SHOULD("%s", "return SAFU_RESULT_FAILED when calling safuAllocMem fails");

    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosSession_t));
    will_return(__wrap_safuAllocMem, NULL);
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);

    result = elosConnectTcpip(MOCK_IP_ADDR, MOCK_PORT, &newSession);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
