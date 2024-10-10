// SPDX-License-Identifier: MIT

#include "elosJsonBackendFindEvents_utest.h"

int elosTestElosJsonBackendFindEventsSuccessSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(elosUteststateT_t));
    assert_non_null(testState);

    safuResultE_t result = elosJsonBackendNew(&testState->backend);
    assert_int_not_equal(result, SAFU_RESULT_FAILED);

    testState->eventVector = safuAllocMem(NULL, sizeof(safuVec_t));
    int res = safuVecCreate(testState->eventVector, NUM_FILTER, sizeof(elosEvent_t));
    assert_int_equal(res, 0);

    *state = testState;

    elosJsonBackendFindEventsUtestCreateEvent(state);
    elosJsonBackendFindEventsUtestCreateFilter(state);

    return 0;
}

int elosTestElosJsonBackendFindEventsSuccessTeardown(void **state) {
    elosUteststateT_t *testState = *state;

    testState->backend->shutdown(testState->backend);

    for (uint32_t idx = 0; idx < testState->eventVector->elementCount; idx++) {
        elosEvent_t *eventEntry = (elosEvent_t *)safuVecGet(testState->eventVector, idx);
        assert_non_null(eventEntry);
        elosEventDeleteMembers(eventEntry);
    }
    safuVecFree(testState->eventVector);
    free(testState->eventVector);

    free(testState->eventJsonString);
    free(testState->filterRules);
    for (int idx = 0; idx < NUM_FILTER; idx++) {
        elosEventFilterDeleteMembers(&testState->filter[idx]);
    }
    free(testState->filter);
    free(testState);
    return 0;
}

void elosTestElosJsonBackendFindEventsSuccess(void **state) {
    elosUteststateT_t *testState = *state;
    const char **filterRules = testState->filterRules;
    safuResultE_t result = SAFU_RESULT_FAILED;

    FILE *mockJsonBackend = NULL;

    TEST("elosJsonBackendFindEvents");
    SHOULD("%s", "test if filtering events from json backend works properly");

    struct timespec nullTime = {0};

    print_message("Test event: %s\n", testState->eventJsonString);

    MOCK_FUNC_ALWAYS(fdopen);
    MOCK_FUNC_ALWAYS(fseek);

    for (int idx = 0; idx < NUM_FILTER; idx++) {
        PARAM("filterrule: %s", filterRules[idx]);

        mockJsonBackend = fmemopen(testState->eventJsonString, testState->eventStrLen, "r");

        expect_value(__wrap_fdopen, fd, 0);
        expect_string(__wrap_fdopen, mode, "r");
        will_return(__wrap_fdopen, mockJsonBackend);

        expect_any(__wrap_fseek, stream);
        expect_value(__wrap_fseek, offset, 0);
        expect_value(__wrap_fseek, origin, SEEK_END);
        will_return(__wrap_fseek, 0);

        MOCK_FUNC_AFTER_CALL(ftell, 0);
        expect_any(__wrap_ftell, stream);
        will_return(__wrap_ftell, testState->eventStrLen);

        result = testState->backend->findEvent(testState->backend, &testState->filter[idx], &nullTime, &nullTime,
                                               testState->eventVector);
        assert_int_not_equal(result, SAFU_RESULT_FAILED);

        fclose(mockJsonBackend);
    }

    MOCK_FUNC_NEVER(fdopen);
    MOCK_FUNC_NEVER(fseek);

    // 2 out of 3 filter shall match the event
    assert_int_equal(testState->eventVector->elementCount, 2);
}
