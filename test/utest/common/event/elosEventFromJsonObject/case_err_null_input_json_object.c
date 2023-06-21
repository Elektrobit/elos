// SPDX-License-Identifier: MIT

#include "elosEventFromJsonObject_utest.h"

int elosTestElosEventFromJsonObjectNUllInputJobjSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventFromJsonObjectNUllInputJobjTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventFromJsonObjectNUllInputJobj(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;
    elosEvent_t event = {0};

    TEST("elosEventFromJsonObject");
    SHOULD("%s", "return SAFU_RESULT_FAILED since empty json object is passed");

    result = elosEventFromJsonObject(&event, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
