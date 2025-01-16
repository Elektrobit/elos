// SPDX-License-Identifier: MIT

#include <fcntl.h>

#include "cmocka_mocks/mock_libc.h"
#include "elosJsonBackendPersist_utest.h"

int elosTestElosJsonBackendPersistSuccessSetup(UNUSED void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate test resources");

    safuResultE_t result = elosJsonBackendNew(&testState->backend);
    assert_int_equal(result, SAFU_RESULT_OK);

    testState->event.date.tv_sec = 25;
    testState->event.date.tv_nsec = 100;
    testState->event.severity = ELOS_SEVERITY_DEBUG;
    testState->event.hardwareid = "localhost";
    testState->event.classification = 42;
    testState->event.messageCode = ELOS_MSG_CODE_NEW_SUBSCRIPTION;
    testState->event.payload = "payload";
    testState->eventSource.appName = "testApp";
    testState->eventSource.fileName = "/bin/testApp";
    testState->eventSource.pid = 123;
    testState->event.source = testState->eventSource;

    elosJsonBackend_t *jsonBackend = testState->backend->backendData;
    char filepattern[] = "/tmp/elosd_2023-04-05_%count%.log";
    char filename[] = "/tmp/elosd_2023-04-05_0.log";
    char date[] = "2023-04-05";
    char dateFormat[] = "%Y-%m-%d";

    jsonBackend->storageFilePattern = safuAllocMem(NULL, sizeof(char) * (strlen(filepattern) + 1));
    jsonBackend->filePath = safuAllocMem(NULL, sizeof(char) * (strlen(filename) + 1));
    testState->filePath = safuAllocMem(NULL, sizeof(char) * (strlen(filename) + 1));
    jsonBackend->storageDate = safuGetCurrentDateString(dateFormat);
    jsonBackend->dateFormat = safuAllocMem(NULL, sizeof(char) * (strlen(dateFormat) + 1));

    strcpy(jsonBackend->storageFilePattern, filepattern);
    strcpy(jsonBackend->filePath, filename);
    strcpy(testState->filePath, filename);
    strcpy(jsonBackend->storageDate, date);
    strcpy(jsonBackend->dateFormat, dateFormat);

    jsonBackend->pathSizeLimit = 1024;
    jsonBackend->additionalFlags = 0;
    jsonBackend->storageFileWrite = WRITE_FD;
    jsonBackend->storageFileRead = READ_FD;
    jsonBackend->count = 0;

    result = elosEventSerialize(&testState->expectedString, &testState->event);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = testState;
    return 0;
}

int elosTestElosJsonBackendPersistSuccessTeardown(UNUSED void **state) {
    elosTestState_t *testState = *state;
    elosJsonBackend_t *jsonBackend = testState->backend->backendData;

    free(jsonBackend->storageFilePattern);
    free(jsonBackend->dateFormat);

    testState->backend->shutdown(testState->backend);
    free(testState->expectedString);
    free(testState->filePath);
    free(testState);
    return 0;
}

void elosTestElosJsonBackendPersistSuccess(UNUSED void **state) {
    elosTestState_t *testState = *state;

    MOCK_FUNC_ALWAYS(safuWriteExactly);
    expect_value(__wrap_safuWriteExactly, fd, WRITE_FD);
    expect_string(__wrap_safuWriteExactly, buf, testState->expectedString);
    expect_value(__wrap_safuWriteExactly, len, strlen(testState->expectedString));
    will_return(__wrap_safuWriteExactly, strlen(testState->expectedString));

    expect_value(__wrap_safuWriteExactly, fd, WRITE_FD);
    expect_string(__wrap_safuWriteExactly, buf, "\n");
    expect_value(__wrap_safuWriteExactly, len, 1);
    will_return(__wrap_safuWriteExactly, 1);

    MOCK_FUNC_ALWAYS(close)
    expect_value(__wrap_close, fd, WRITE_FD);
    will_return(__wrap_close, 0);
    expect_value(__wrap_close, fd, READ_FD);
    will_return(__wrap_close, 0);

    MOCK_FUNC_ALWAYS(open)
    expect_string(__wrap_open, file, testState->filePath);
    expect_value(__wrap_open, flags, O_CREAT | O_WRONLY | O_APPEND | O_CLOEXEC);
    expect_value(__wrap_open, mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    will_return(__wrap_open, WRITE_FD);
    expect_string(__wrap_open, file, testState->filePath);
    expect_value(__wrap_open, flags, O_RDONLY | O_CLOEXEC);
    expect_value(__wrap_open, mode, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    will_return(__wrap_open, READ_FD);

    safuResultE_t result = testState->backend->persist(testState->backend, &testState->event);

    MOCK_FUNC_NEVER(close)
    MOCK_FUNC_NEVER(open)
    MOCK_FUNC_NEVER(safuWriteExactly);

    assert_int_equal(result, SAFU_RESULT_OK);
}
