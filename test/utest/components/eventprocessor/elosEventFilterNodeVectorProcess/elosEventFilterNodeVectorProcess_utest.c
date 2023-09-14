// SPDX-License-Identifier: MIT
#include "elosEventFilterNodeVectorProcess_utest.h"

#define _NODES 2

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestEloEventFilterNodeVectorProcessErrIter),
        TEST_CASE(elosTestEloEventFilterNodeVectorProcessErrParam),
        TEST_CASE(elosTestEloEventFilterNodeVectorProcessSuccessWithVStack),
        TEST_CASE(elosTestEloEventFilterNodeVectorProcessSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(elosTestState_t));
    return 0;
}

static int _testSuiteTeardown(void **state) {
    free(*state);
    return 0;
}

void elosEventFilterNodeVectorSetup(elosTestState_t *test, char const *fStrA, char const *fStrB) {
    elosEventFilterNodeParam_t fNodeParamA = {.filterStrings = &fStrA, .filterStringCount = 1, .id = 1};
    elosEventFilterNodeParam_t fNodeParamB = {.filterStrings = &fStrB, .filterStringCount = 1, .id = 2};
    elosEventQueueParam_t const eqParamA = _EQ_PARAM(42);
    elosEventQueueParam_t const eqParamB = _EQ_PARAM(43);
    safuResultE_t result;

    memset(test, 0, sizeof(elosTestState_t));

    result = elosEventFilterNodeVectorInitialize(&test->eventFilterNodeVector, _NODES);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventQueueNew(&fNodeParamA.eventQueue, &eqParamA);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventQueueNew(&fNodeParamB.eventQueue, &eqParamB);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventFilterNodeVectorPushByParam(&test->eventFilterNodeVector, &fNodeParamA);
    assert_true(result == SAFU_RESULT_OK);
    result = elosEventFilterNodeVectorPushByParam(&test->eventFilterNodeVector, &fNodeParamB);
    assert_true(result == SAFU_RESULT_OK);
}

void elosEventFilterNodeVectorTeardown(elosTestState_t *test) {
    elosEventFilterNode_t *efn;
    uint32_t elements;

    elements = safuVecElements(&test->eventFilterNodeVector);

    for (uint32_t idx = 0; idx < elements; idx += 1) {
        efn = safuVecGet(&test->eventFilterNodeVector, idx);
        if (efn != NULL) {
            elosEventQueueDelete(efn->eventQueue);
        }
    }

    elosEventFilterNodeVectorDeleteMembers(&test->eventFilterNodeVector);
}

void elosEventFilterNodeVectorTestStep(elosTestState_t *test, char const *message, elosEvent_t const *event,
                                       elosEventFilterStack_t const *vStack, uint32_t sizeA, uint32_t sizeB) {
    elosEventFilterNode_t *testNode;
    safuResultE_t result;

    PARAM("%s", message);
    result = elosEventFilterNodeVectorProcess(&test->eventFilterNodeVector, vStack, event);
    assert_true(result == SAFU_RESULT_OK);

    testNode = (elosEventFilterNode_t *)safuVecGet(&test->eventFilterNodeVector, 0);
    assert_non_null(testNode);
    assert_int_equal(testNode->eventQueue->eventVector->elementCount, sizeA);

    testNode = (elosEventFilterNode_t *)safuVecGet(&test->eventFilterNodeVector, 1);
    assert_non_null(testNode);
    assert_int_equal(testNode->eventQueue->eventVector->elementCount, sizeB);
}
