// SPDX-License-Identifier: MIT

#include "elosLog_utest.h"

int elosTestElosLogLoggerUninitializedSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosLogLoggerUninitializedTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosLogLoggerUninitialized(UNUSED void **state) {
    const char *testString = "logger test";

    TEST("elosLog");
    SHOULD("%s", "log message to fallback");

    elosLog(ELOS_MSG_CODE_UNAUTHORIZED_PUBLISHING, ELOS_SEVERITY_WARN,
            ELOS_CLASSIFICATION_SECURITY | ELOS_CLASSIFICATION_ELOS | ELOS_CLASSIFICATION_IPC, testString);
}
