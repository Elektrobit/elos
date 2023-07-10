// SPDX-License-Identifier: MIT

#include <cmocka_mocks/mock_libc.h>
#include <safu/mock_vector.h>
#include <signal.h>

#include "elosEventDispatcherBufferAdd_utest.h"

int testElosEventDispatcherBufferAddErrFuncSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int testElosEventDispatcherBufferAddErrFuncTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    elosEventDispatcherDeleteMembers(&test->eventDispatcher);

    return 0;
}

void testElosEventDispatcherBufferAddErrFunc(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherBufferAdd");
    SHOULD("test behaviour when internal functions fail");

    PARAM("safuVecPush fails");

    MOCK_FUNC_AFTER_CALL(safuVecPush, 0);
    expect_any(__wrap_safuVecPush, vec);
    expect_any(__wrap_safuVecPush, element);
    will_return(__wrap_safuVecPush, -1);

    result = elosEventDispatcherBufferAdd(&test->eventDispatcher, &test->eventBuffer[0]);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
