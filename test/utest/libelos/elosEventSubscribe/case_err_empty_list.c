// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeErrEmptyListSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventSubscribeErrEmptyListTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventSubscribeErrEmptyList(UNUSED void **state) {
    elosSession_t session = {.connected = true};
    elosEventQueueId_t eventQueueId = ELOS_ID_INVALID;
    safuResultE_t result;

    TEST("elosEventSubscribe");
    SHOULD("%s", "Return SAFU_RESULT_FAILED when passing an empty filterStringList");

    const char *filter[] = {
        NULL,
    };

    result = elosEventSubscribe(&session, filter, ARRAY_SIZE(filter), &eventQueueId);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(eventQueueId, ELOS_ID_INVALID);
}
