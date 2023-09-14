// SPDX-License-Identifier: MIT

#include "elosEventSerialize_utest.h"

int elosTestElosEventSerializeJsonObjStringErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSerializeJsonObjStringErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSerializeJsonObjStringError(UNUSED void **state) {
    char *result = NULL;
    const elosEvent_t testEvent = {.severity = ELOS_SEVERITY_DEBUG};
    safuResultE_t retval = SAFU_RESULT_OK;

    TEST("elosEventSerialize");
    SHOULD("%s", "return SAFU_RESULT_FAILED as json object to string conversion fails");

    MOCK_FUNC_AFTER_CALL(json_object_to_json_string_ext, 0);
    expect_any(__wrap_json_object_to_json_string_ext, obj);
    expect_any(__wrap_json_object_to_json_string_ext, flags);
    will_return(__wrap_json_object_to_json_string_ext, NULL);

    retval = elosEventSerialize(&result, &testEvent);
    assert_int_equal(retval, SAFU_RESULT_FAILED);
}
