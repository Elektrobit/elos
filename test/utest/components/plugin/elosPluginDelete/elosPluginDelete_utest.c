// SPDX-License-Identifier: MIT

#include "elosPluginDelete_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosPluginDeleteErrParam),
        TEST_CASE(elosTestElosPluginDeleteSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    *state = NULL;

    return 0;
}

char const *elosPluginFuncDefaultName[ELOS_PLUGIN_FUNC_COUNT] = {
    [ELOS_PLUGIN_FUNC_LOAD] = "elosPluginLoad",
    [ELOS_PLUGIN_FUNC_START] = "elosPluginStart",
    [ELOS_PLUGIN_FUNC_STOP] = "elosPluginStop",
    [ELOS_PLUGIN_FUNC_UNLOAD] = "elosPluginUnload",
};
