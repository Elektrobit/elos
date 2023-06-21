// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"
#include "mock_event.h"

int elosTestElosEventSerializeNewJsonObjectErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeNewJsonObjectErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeNewJsonObjectError(UNUSED void **state) {
    char *result = NULL;
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};
    safuResultE_t retval = SAFU_RESULT_OK;

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as creating a new json object fails");

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
