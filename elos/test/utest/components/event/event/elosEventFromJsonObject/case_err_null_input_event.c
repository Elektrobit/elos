// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"

int elosTestElosEventFromJsonObjectNUllInputEventSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventFromJsonObjectNUllInputEventTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventFromJsonObjectNUllInputEvent(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    json_object *jobj = json_object_new_object();
    assert_non_null(jobj);

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED since empty event is passed");

    result = elosEventFromJsonObject(NULL, jobj);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    json_object_put(jobj);
}
