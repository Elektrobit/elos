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
    ssize_t fileNameSize = 0;
    elosLogStatusE_t result = ELOS_LOG_STATUS_ERROR;
    elosEvent_t *testEvent = NULL;
    elosLogData_t *testLog = NULL;
    const char *testMessage = "testMessage";
    char *appName = NULL;
    char *fileName = NULL;

    TEST("elosLogCreateElosEventFromLog");
    SHOULD("%s", "creates an elos event from log parameters");

    pid_t testpid = getpid();

    fileName = safuAllocMem(NULL, PATH_MAX);
    fileNameSize = readlink("/proc/self/exe", fileName, PATH_MAX);

    fileName[fileNameSize] = 0;
    appName = strdup(strrchr(fileName, '/'));
    fileName = safuAllocMem(fileName, fileNameSize + sizeof('\0'));

    elosLogCreateLogData(ELOS_MSG_CODE_DEBUG_LOG, ELOS_SEVERITY_DEBUG, ELOS_CLASSIFICATION_PROCESS, testMessage,
                         &testLog);
    assert_non_null(testLog);

    result = elosLogCreateElosEventFromLog(testLog, &testEvent);

    assert_int_equal(result, ELOS_LOG_STATUS_SUCCESS);
    assert_int_not_equal(testEvent->date.tv_sec, 0);
    assert_int_not_equal(testEvent->date.tv_nsec, 0);
    assert_string_equal(testEvent->source.appName, appName);
    assert_string_equal(testEvent->source.fileName, fileName);
    assert_int_equal(testEvent->source.pid, testpid);
    assert_int_equal(testEvent->messageCode, ELOS_MSG_CODE_DEBUG_LOG);
    assert_int_equal(testEvent->severity, ELOS_SEVERITY_DEBUG);
    assert_int_equal(testEvent->classification, ELOS_CLASSIFICATION_PROCESS);
    assert_string_equal(testEvent->payload, testMessage);

    free(fileName);
    free(appName);
    elosLogDeleteLogData(testLog);
    elosEventDelete(testEvent);
}
