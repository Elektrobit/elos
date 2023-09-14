// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <safu/common.h>
#include <safu/mock_safu.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeExterrJsonAddNewStringFailedSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeExterrJsonAddNewStringFailedTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState);
    return 0;
}

void elosTestElosEventSubscribeExterrJsonAddNewStringFailed(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "try to create a subscription message and catch an error of safuJsonAddNewString.");

    struct _TestState *testState = *state;
    elosSession_t *expectedSession = &testState->session;

    MOCK_FUNC_AFTER_CALL(safuJsonAddNewString, 1);
    expect_any(__wrap_safuJsonAddNewString, jobj);
    expect_value(__wrap_safuJsonAddNewString, name, NULL);
    expect_string(__wrap_safuJsonAddNewString, val, "event.source.appName,'linux',eq");
    will_return(__wrap_safuJsonAddNewString, NULL);

    const char *filter[] = {
        "event.source.appName,'myApp',eq",
        "event.source.appName,'linux',eq",
        "event.source.appName,'openssh',eq",
        NULL,
    };
    elosEventQueueId_t id = ELOS_ID_INVALID;

    safuResultE_t result = elosEventSubscribe(expectedSession, filter, ARRAY_SIZE(filter), &id);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(id, ELOS_ID_INVALID);
}
