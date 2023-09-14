// SPDX-License-Identifier: MIT

#include "elosEventBufferDeleteMembers_utest.h"

#define _TEST_EVENTLIMITCOUNT 8

int elosTestElosEventBufferDeleteMembersSuccessSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventBufferParam_t const param = {.limitEventCount = _TEST_EVENTLIMITCOUNT};
    safuResultE_t result;

    result = elosEventBufferInitialize(&test->eventBuffer, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestElosEventBufferDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventBufferDeleteMembersSuccess(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    TEST("elosEventBufferDeleteMembers");
    SHOULD("%s", "test correct behaviour of elosEventBufferDeleteMembers");

    result = elosEventBufferDeleteMembers(&test->eventBuffer);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_int_equal(SAFU_FLAG_HAS_INITIALIZED_BIT(&test->eventBuffer.flags), false);
    assert_int_equal(test->eventBuffer.ringCount, 0);
    assert_ptr_equal(test->eventBuffer.ring, NULL);
}
