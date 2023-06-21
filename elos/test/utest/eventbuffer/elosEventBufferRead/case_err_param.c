// SPDX-License-Identifier: MIT

#include "elosEventBufferRead_utest.h"

#define _TEST_EVENT_LIMIT 8

typedef struct _testEntry {
    char *description;
    elosEventBuffer_t *eventBuffer;
    size_t priority;
    elosEventVector_t *eventPtrVec;
    size_t *elementsWritten;
} _testEntry_t;

int elosTestElosEventBufferReadErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENT_LIMIT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferReadErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventBufferDeleteMembers(&test->eventBuffer);

    return 0;
}

void elosTestElosEventBufferReadErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    size_t const invalidPriority = ELOS_EVENTBUFFER_PRIORITY_COUNT;
    size_t const validPriority = ELOS_EVENTBUFFER_PRIORITY_NORMAL;
    elosEventBuffer_t invalidEventBuffer = {0};
    safuResultE_t result;

    // elementsWritten can be zero, so it is not tested here
    _testEntry_t const testTable[] = {
        {"NULL, invalidPriority, NULL, NULL", NULL, invalidPriority, NULL, NULL},
        {"&invalidEventBuffer, invalidPriority, NULL, NULL", &invalidEventBuffer, invalidPriority, NULL, NULL},
        {"&eventBuffer, invalidPriority, NULL, NULL", &test->eventBuffer, invalidPriority, NULL, NULL},
        {"&eventBuffer, validPriority, NULL, NULL", &test->eventBuffer, validPriority, NULL, NULL},
    };

    TEST("elosEventBufferRead");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    for (size_t i = 0; i < ARRAY_SIZE(testTable); i += 1) {
        _testEntry_t const *entry = &testTable[i];

        PARAM("%s", entry->description);
        result = elosEventBufferRead(entry->eventBuffer, entry->priority, entry->eventPtrVec, entry->elementsWritten);
        assert_int_equal(result, SAFU_RESULT_FAILED);
    }
}
