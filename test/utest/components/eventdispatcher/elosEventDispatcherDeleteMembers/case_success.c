// SPDX-License-Identifier: MIT

#include "elosEventDispatcherDeleteMembers_utest.h"

int testElosEventDispatcherDeleteMembersSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventDispatcherParam_t const param = {.eventProcessor = &test->eventProcessor};
    safuResultE_t result;

    result = elosEventDispatcherInitialize(&test->eventDispatcher, &param);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_ptr_not_equal(test->eventDispatcher.eventBufferPtrVector.data, NULL);

    return 0;
}

int testElosEventDispatcherDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void testElosEventDispatcherDeleteMembersSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventDispatcherDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventDispatcherDeleteMembers");

    result = elosEventDispatcherDeleteMembers(&test->eventDispatcher);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->eventDispatcher.flags), false);
    assert_ptr_equal(test->eventDispatcher.eventBufferPtrVector.data, NULL);
}
