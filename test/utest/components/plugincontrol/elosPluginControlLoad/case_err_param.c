// SPDX-License-Identifier: MIT

#include "elosPluginControlLoad_utest.h"

int elosTestElosPluginControlLoadErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosPluginControlLoadErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosPluginControlLoadErrParam(UNUSED void **state) {
    elosPlugin_t zeroedPlugin = {0};
    safuResultE_t result;

    TEST("elosPluginControlLoad");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL");
    result = elosPluginControlLoad(NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "zeroedPlugin, NULL");
    result = elosPluginControlLoad(&zeroedPlugin);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
