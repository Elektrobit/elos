// SPDX-License-Identifier: MIT
#include "pluginUnload_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(pluginUnloadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestPluginUnloadSuccess),
        TEST_CASE(elosTestPluginUnloadErrInvalidPlugin),
    };

    return RUN_TEST_SUITE(tests, pluginUnloadUtest);
}

static int pluginUnloadUtestSetup(UNUSED void **state) {
    return 0;
}

static int pluginUnloadUtestTeardown(UNUSED void **state) {
    return 0;
}
