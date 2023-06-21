// SPDX-License-Identifier: MIT

#include "elosLogFindEvent_utest.h"

int elosTestElosLogFindEventErrSessionSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLogFindEventErrSessionTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLogFindEventErrSession(UNUSED void **state) {
    const char filterRule[] = ".event.name 'sshd' STRCMP";
    elosEventVector_t *vector = NULL;
    safuResultE_t result;

    TEST("elosLogFindEvent");
    SHOULD("%s", "return SAFU_RESULT_FAILED when the session parameter is null");

    result = elosLogFindEvent(NULL, filterRule, &vector);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
