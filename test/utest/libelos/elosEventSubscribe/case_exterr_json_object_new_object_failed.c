// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>
#include <elos/libelos/libelos.h>
#include <safu/common.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeExterrJsonObjectNewObjectFailedSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeExterrJsonObjectNewObjectFailedTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState->request.message);
    free(testState->response.message);
    free(testState);
    return 0;
}

void elosTestElosEventSubscribeExterrJsonObjectNewObjectFailed(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "try to create a subscription message and catch an error of json_object_new_object");

    struct _TestState *testState = *state;
    elosSession_t *expectedSession = &testState->session;

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);
    will_return(__wrap_json_object_new_object, NULL);

    const char *filter[] = {
        "event.source.appName,'myApp',eq",
        "event.source.appName,'linux',eq",
        "event.source.appName,'openssh',eq",
    };
    elosEventQueueId_t id = ELOS_ID_INVALID;

    safuResultE_t result = elosEventSubscribe(expectedSession, filter, ARRAY_SIZE(filter), &id);

    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_int_equal(id, ELOS_ID_INVALID);
}
