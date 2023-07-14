// SPDX-License-Identifier: MIT

#include "elosEventToJsonObject_utest.h"

int elosTestElosEventToJsonObjectEventNullErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventToJsonObjectEventNullErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventToJsonObjectEventNullError(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    json_object *resultObject = json_object_new_object();
    assert_non_null(resultObject);

    TEST("elosEventToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED if 'from' is null");

    result = elosEventToJsonObject(resultObject, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    json_object_put(resultObject);
}
