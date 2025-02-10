// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_vector.h>
#include <signal.h>

#include "elosEventDispatcherBufferRemove_utest.h"

int testElosEventDispatcherBufferRemoveErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherBufferRemoveErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferRemoveErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    test->eventBuffer[0].permitRemoval = true;

    TEST("elosEventDispatcherBufferRemove");
    SHOULD("test behaviour when internal functions fail");

    PARAM("safuVecFindRemove fails");

    MOCK_FUNC_AFTER_CALL(safuVecFindRemove, 0);
    expect_any(__wrap_safuVecFindRemove, vec);
    expect_any(__wrap_safuVecFindRemove, matchFunc);
    expect_any(__wrap_safuVecFindRemove, matchData);
    will_return(__wrap_safuVecFindRemove, -1);

    result = elosEventDispatcherBufferRemove(&test->eventDispatcher, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
