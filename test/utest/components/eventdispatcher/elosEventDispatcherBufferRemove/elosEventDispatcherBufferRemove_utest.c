// SPDX-License-Identifier: MIT
#include "elosEventDispatcherBufferRemove_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(testElosEventDispatcherBufferRemoveErrFunc),
        TEST_CASE(testElosEventDispatcherBufferRemoveErrMutex),
        TEST_CASE(testElosEventDispatcherBufferRemoveErrParam),
        TEST_CASE(testElosEventDispatcherBufferRemoveSuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;
    safuResultE_t result;

    test = calloc(1, sizeof(elosUnitTestState_t));
    assert_non_null(test);

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferParam_t const param = {.limitEventCount = (i + 1)};
        result = elosEventBufferInitialize(&test->eventBuffer[i], &param);
        assert_int_equal(result, SAFU_RESULT_OK);
    }

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    for (size_t i = 0; i < _EVENTBUFFER_ENTRIES; i += 1) {
        elosEventBufferDeleteMembers(&test->eventBuffer[i]);
    }

    free(*state);
    *state = NULL;

    return 0;
}
