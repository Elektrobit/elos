// SPDX-License-Identifier: MIT

#include "elosEventVectorToJsonArray_utest.h"

#include "safu/common.h"

#define _TEST_EVENT_COUNT 2

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventVectorToJsonArrayErrArrayAdd),
        TEST_CASE(elosTestElosEventVectorToJsonArrayErrArrayNew),
        TEST_CASE(elosTestElosEventVectorToJsonArrayErrParam),
        TEST_CASE(elosTestElosEventVectorToJsonArraySuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    elosEvent_t event = {0};
    safuResultE_t result;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);
    memset(test, 0, sizeof(elosUnitTestState_t));

    result = elosEventVectorNew(&test->eventVector, _TEST_EVENT_COUNT);
    assert_int_equal(result, SAFU_RESULT_OK);

    event.messageCode = ELOS_MSG_CODE_WILDCARD_7XXX;
    safuVecPush(test->eventVector, &event);

    event.messageCode = ELOS_MSG_CODE_WILDCARD_8XXX;
    safuVecPush(test->eventVector, &event);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventVectorDelete(test->eventVector);
    free(*state);

    return 0;
}
