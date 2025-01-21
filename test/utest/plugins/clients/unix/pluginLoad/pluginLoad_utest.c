// SPDX-License-Identifier: MIT
#include "pluginLoad_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(pluginLoadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestPluginLoadSuccess),
        TEST_CASE(elosTestPluginLoadErrInvalidPlugin),
    };

    return RUN_TEST_SUITE(tests, pluginLoadUtest);
}

static int pluginLoadUtestSetup(UNUSED void **state) {
    return 0;
}

static int pluginLoadUtestTeardown(UNUSED void **state) {
    return 0;
}
