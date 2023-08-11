// SPDX-License-Identifier: MIT

#include "elosPluginLoad_utest.h"

int elosTestElosPluginLoadErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginLoadErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginLoadErrParam(UNUSED void **state) {
    elosPlugin_t zeroedPlugin = {0};
    safuResultE_t result;

    TEST("elosPluginLoad");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosPluginLoad(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedPlugin, NULL");
    result = elosPluginLoad(&zeroedPlugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
