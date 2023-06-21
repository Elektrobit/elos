// SPDX-License-Identifier: MIT

#include "elosCreateMessage_utest.h"
#include "safu/mock_safu.h"

int elosTestElosCreateMessageErrAllocationSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    test->message = NULL;

    return 0;
}

int elosTestElosCreateMessageErrAllocationTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    free(test->message);
    test->message = NULL;

    return 0;
}

void elosTestElosCreateMessageErrAllocation(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    char const *jsonStr = "{}";
    size_t const length = strlen(jsonStr) + 1;

    TEST("elosCreateMessage");
    SHOULD("%s", "test correct behaviour of elosCreateMessage on memory allocation fail");

    MOCK_FUNC_AFTER_CALL(safuAllocMem, 0);
    expect_value(__wrap_safuAllocMem, oldptr, NULL);
    expect_value(__wrap_safuAllocMem, newlen, sizeof(elosMessage_t) + length);
    will_return(__wrap_safuAllocMem, NULL);

    result = elosCreateMessage(ELOS_MESSAGE_GET_VERSION, jsonStr, &test->message);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(test->message, NULL);
}
