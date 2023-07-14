// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"
#include "mock_event.h"

int elosTestElosEventSerializeEventConversionErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeEventConversionErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeEventConversionError(UNUSED void **state) {
    char *result = NULL;
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};
    safuResultE_t retval = SAFU_RESULT_OK;

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as json object to string conversion fails");

    MOCK_FUNC_AFTER_CALL(elosEventToJsonObject, 0);
    expect_any(elosEventToJsonObject, to);
    expect_any(elosEventToJsonObject, from);
    will_return(elosEventToJsonObject, SAFU_RESULT_FAILED);

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
