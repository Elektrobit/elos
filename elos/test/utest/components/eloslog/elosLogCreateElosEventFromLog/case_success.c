// SPDX-License-Identifier: MIT

#include <linux/limits.h>
#include <safu/common.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogSuccess(UNUSED void **state) {
    elosEvent_t testEvent = {0};
    const char *testMessage = "testMessage";

    pid_t testpid = getpid();

    TEST("elosLogCreateElosEventFromLog");
    SHOULD("%s", "creates an elos event from log parameters");

    elosLogCreateElosEventFromLog(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS,
                                  testMessage, &testEvent);

    assert_int_not_equal(testEvent.date.tv_sec, 0);
    assert_int_not_equal(testEvent.date.tv_nsec, 0);
    assert_int_equal(testEvent.source.pid, testpid);
    assert_int_equal(testEvent.messageCode, ELOS_MSG_CODE_DEBUG_LOG);
    assert_int_equal(testEvent.severity, ELOS_SEVERITY_DEBUG);
    assert_int_equal(testEvent.classification, ELOS_CLASSIFICATION_PROCESS);
    assert_string_equal(testEvent.payload, testMessage);

    elosEventDeleteMembers(&testEvent);
}
