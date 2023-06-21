// SPDX-License-Identifier: MIT

#include <pthread.h>

#include "safuRingBufferWrite_utest.h"

#define _TEST_WORKERS    2
#define _TEST_ELEMENTS   5
#define _TEST_ITERATIONS ((_TEST_ELEMENTS * 100) + 1)

void *_workerThread(void *state) {
    safuUnitTestState_t *test = (safuUnitTestState_t *)state;

    for (size_t i = 0; i < _TEST_ITERATIONS; i += 1) {
        size_t const idx = (i % _TEST_ELEMENTS);
        safuResultE_t result;

        result = safuRingBufferWrite(&test->ringBuffer, &test->values[idx]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    return NULL;
}

int safuTestSafuRingBufferWriteSuccessThreadedSetup(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    safuRingBufferParam_t const param = {.elements = _TEST_ELEMENTS};
    safuResultE_t result;

    result = safuRingBufferInitialize(&test->ringBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int safuTestSafuRingBufferWriteSuccessThreadedTeardown(UNUSED void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;

    safuRingBufferDeleteMembers(&test->ringBuffer);

    return 0;
}

void safuTestSafuRingBufferWriteSuccessThreaded(void **state) {
    safuUnitTestState_t *test = *(safuUnitTestState_t **)state;
    size_t testValue[_TEST_ELEMENTS] = {0};
    pthread_t worker[_TEST_WORKERS] = {0};

    for (size_t i = 0; i < _TEST_ELEMENTS; i += 1) {
        testValue[i] = i + 1;
    }

    test->values = testValue;

    TEST("safuRingBufferWrite");
    SHOULD("%s", "test correct behaviour of multi-threaded safuRingBufferWrite");

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        int retVal;

        retVal = pthread_create(&worker[i], NULL, _workerThread, *state);
        assert_int_equal(retVal, 0);
    }

    for (size_t i = 0; i < _TEST_WORKERS; i += 1) {
        int retVal;

        retVal = pthread_join(worker[i], NULL);
        assert_int_equal(retVal, 0);
    }

    size_t const elementsWritten = (_TEST_ITERATIONS * _TEST_WORKERS);
    size_t const finalIdx = elementsWritten % 5;

    assert_int_equal(test->ringBuffer.nextIdx, finalIdx);
    assert_int_equal(test->ringBuffer.elementsWritten, elementsWritten);
}
