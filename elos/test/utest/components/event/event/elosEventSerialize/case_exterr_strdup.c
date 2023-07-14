// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeStrdupErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeStrdupErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeStrdupError(UNUSED void **state) {
    char *result = NULL;
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};
    safuResultE_t retval = SAFU_RESULT_OK;

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED if the allocation of the result string fails");

    MOCK_FUNC_AFTER_CALL(strdup, 0);
    expect_any(__wrap_strdup, string);
    will_return(__wrap_strdup, NULL);

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
