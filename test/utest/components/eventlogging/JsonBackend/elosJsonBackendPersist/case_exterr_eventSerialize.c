// SPDX-License-Identifier: MIT

#include "elosJsonBackendPersist_utest.h"
#include "mock_event.h"

int elosTestElosJsonBackendPersistExterrEventSerializeSetup(UNUSED void **state) {
    elosTestState_t *testState = safuAllocMem(NULL, sizeof(elosTestState_t));
    assert_non_null_msg(testState, "Failed to allocate basic test state");

    safuResultE_t result = elosJsonBackendNew(&testState->backend);
    assert_non_null_msg(testState->backend, "Failed to create test backend");
    assert_int_equal(result, SAFU_RESULT_OK);

    elosJsonBackend_t *jsonBackend = testState->backend->backendData;
    char filename[] = "/tmp/elosd.log";
    char dateFormat[] = "%Y-%m-%d";

    jsonBackend->storageFilePattern = safuAllocMem(NULL, sizeof(char) * (strlen(filename) + 1));
    jsonBackend->filePath = safuAllocMem(NULL, sizeof(char) * (strlen(filename) + 1));
    jsonBackend->storageDate = safuGetCurrentDateString(dateFormat);
    jsonBackend->dateFormat = safuAllocMem(NULL, sizeof(char) * (strlen(dateFormat) + 1));

    strcpy(jsonBackend->storageFilePattern, filename);
    strcpy(jsonBackend->filePath, filename);
    strcpy(jsonBackend->dateFormat, dateFormat);

    jsonBackend->pathSizeLimit = 1024;
    jsonBackend->additionalFlags = 0;
    jsonBackend->storageFileWrite = WRITE_FD;
    jsonBackend->storageFileRead = READ_FD;
    jsonBackend->count = 0;

    *state = testState;
    return 0;
}

int elosTestElosJsonBackendPersistExterrEventSerializeTeardown(UNUSED void **state) {
    elosTestState_t *testState = *state;
    elosJsonBackend_t *jsonBackend = testState->backend->backendData;

    free(jsonBackend->storageFilePattern);
    free(jsonBackend->dateFormat);

    testState->backend->shutdown(testState->backend);
    free(testState);
    return 0;
}

void elosTestElosJsonBackendPersistExterrEventSerialize(void **state) {
    elosTestState_t *testState = *state;

    MOCK_FUNC_AFTER_CALL(elosEventSerialize, 0);
    expect_not_value(elosEventSerialize, toJsonString, NULL);
    expect_value(elosEventSerialize, from, &testState->event);
    will_return(elosEventSerialize, SAFU_RESULT_FAILED);

    /* Should not open or close here, since the date will be the same, and the file should not rotate */
    safuResultE_t result = testState->backend->persist(testState->backend, &testState->event);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
