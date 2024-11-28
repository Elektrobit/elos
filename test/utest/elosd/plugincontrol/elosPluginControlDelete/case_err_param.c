// SPDX-License-Identifier: MIT

#include "elosPluginControlDelete_utest.h"

int elosTestElosPluginControlDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginControlDeleteErrParam(UNUSED void **state) {
    elosPluginControl_t *zeroedPluginPtr = NULL;
    safuResultE_t result;

    TEST("elosPluginControlDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosPluginControlDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "zeroedPluginPtr");
    result = elosPluginControlDelete(&zeroedPluginPtr);
    assert_int_equal(result, SAFU_RESULT_OK);
}
