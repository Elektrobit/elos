// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonObject_utest.h"

int elosTestElosEventVectorToJsonObjectErrEventVectorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventVectorToJsonObjectErrEventVectorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventVectorToJsonObjectErrEventVector(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    json_object *eventVecJobj = NULL;

    TEST("elosEventVectorToJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED when eventVector is null");

    result = elosEventVectorToJsonObject(NULL, &eventVecJobj);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}