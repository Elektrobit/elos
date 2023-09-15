// SPDX-License-Identifier: MIT

#include <linux/limits.h>
#include <safu/common.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include "elosLogCreateElosEventFromLog_utest.h"

int elosLogCreateElosEventFromLogErrParamEventSetup(UNUSED void **state) {
    return 0;
}

int elosLogCreateElosEventFromLogErrParamEventTeardown(UNUSED void **state) {
    return 0;
}

void elosLogCreateElosEventFromLogErrParamEvent(UNUSED void **state) {
    elosEvent_t testEvent = {0};
    pid_t testpid = getpid();

    TEST("elosLogCreateElosEventFromLog");
    SHOULD("%s", "create an elos event with empty payload");

    elosLogCreateElosEventFromLog(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, NULL,
                                  &testEvent);

    assert_int_not_equal(testEvent.date.tv_sec, 0);
    assert_int_not_equal(testEvent.date.tv_nsec, 0);
    assert_int_equal(testEvent.source.pid, testpid);
    assert_int_equal(testEvent.messageCode, ELOS_MSG_CODE_DEBUG_LOG);
    assert_int_equal(testEvent.severity, ELOS_SEVERITY_DEBUG);
    assert_int_equal(testEvent.classification, ELOS_CLASSIFICATION_PROCESS);
    assert_null(testEvent.payload);

    elosEventDeleteMembers(&testEvent);
}
