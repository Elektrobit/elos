// SPDX-License-Identifier: MIT

#include <errno.h>

#include "elosReceiveMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosReceiveMessageErrAllocationSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosReceiveMessageErrAllocationTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosReceiveMessageErrAllocation(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosReceiveSet_t testSet;
    safuResultE_t result;

    TEST("elosReceiveMessage");
    SHOULD("%s", "test correct behaviour of elosReceiveMessage with memory allocation errors");

    PARAM("%s", "safuAllocMem for the header fails");
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosMessage_t));
    will_return(__wrap_safuAllocMem, NULL);

    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);

    PARAM("%s", "safuAllocMem for the body fails");
    testSet = test->receiveJson;
    MOCK_FUNC_AFTER_CALL(safuAllocMem, 1);
    expect_not_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, testSet.messageSize);
    will_return(__wrap_safuAllocMem, NULL);

    elosMockReceiveExactlySetup(&testSet.header, NULL);
    result = elosReceiveMessage(&test->session, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(test->message);
}
