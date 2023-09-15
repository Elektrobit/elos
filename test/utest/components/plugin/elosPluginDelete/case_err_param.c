// SPDX-License-Identifier: MIT

#include "elosPluginDelete_utest.h"

int elosTestElosPluginDeleteErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginDeleteErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginDeleteErrParam(UNUSED void **state) {
    elosPlugin_t *zeroedPluginPtr = NULL;
    safuResultE_t result;

    TEST("elosPluginDelete");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosPluginDelete(NULL);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("%s", "zeroedPluginPtr");
    result = elosPluginDelete(&zeroedPluginPtr);
    assert_int_equal(result, SAFU_RESULT_OK);
}
