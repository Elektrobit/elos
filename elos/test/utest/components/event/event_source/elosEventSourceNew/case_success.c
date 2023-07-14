// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "elos/event/event_source.h"
#include "elosEventSourceNew_utest.h"

int elosTestElosEventSourceNewSuccessSetup(void **state) {
    static elosTestState_t test = {0};
    *state = &test;
    return 0;
}

int elosTestElosEventSourceNewSuccessTeardown(void **state) {
    elosTestState_t *test = *(elosTestState_t **)state;
    elosEventSourceDelete(test->source);
    return 0;
}

void elosTestElosEventSourceNewSuccess(void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosTestState_t *test = *(elosTestState_t **)state;

    TEST("elosEventSourceNew");
    SHOULD("%s", "allocate successfully an elosEventSource_t");

    result = elosEventSourceNew(&test->source);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(test->source);
    assert_null(test->source->appName);
    assert_null(test->source->fileName);
    assert_int_equal(test->source->pid, 0);
}
