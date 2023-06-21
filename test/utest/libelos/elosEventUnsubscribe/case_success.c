// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventUnsubscribe_utest.h"

void elosTestElosEventUnsubscribeSuccess(void **state) {
    safuResultE_t result;
    elosTestState_t *test = *state;

    TEST("elosEventUnsubscribe");
    SHOULD("%s", "Successfully unsubscribe from an event queue.");

    elosMockSendAndReceiveJsonMessage(test, "{\"error\":null}", SAFU_RESULT_OK);

    result = elosEventUnsubscribe(&test->session, _MOCK_EVENTQUEUE_ID);

    assert_int_equal(result, SAFU_RESULT_OK);
}
