// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_extensions/mock_extensions.h>
#include <cmocka_mocks/mock_jsonc.h>
#include <elos/libelos/libelos.h>
#include <safu/common.h>

#include "elosEventSubscribe_utest.h"

int elosTestElosEventSubscribeErrInvalidParameterSetup(void **state) {
    *state = safuAllocMem(NULL, sizeof(struct _TestState));
    memset(*state, 0, sizeof(struct _TestState));

    struct _TestState *testState = *state;
    testState->session.connected = true;

    return 0;
}

int elosTestElosEventSubscribeErrInvalidParameterTeardown(void **state) {
    struct _TestState *testState = *state;

    free(testState->request.message);
    free(testState->response.message);
    free(testState);
    return 0;
}

static void _testElosEventSubscribeErrInvalidParameterParam(elosSession_t *session, const char **filters,
                                                            size_t filterNumber, elosEventQueueId_t *id) {
    PARAM("session is %s, filters is %s, ids is %s", session ? "not null" : "null", filters ? "not null" : "null",
          id ? "not null" : "null");

    safuResultE_t result = elosEventSubscribe(session, filters, filterNumber, id);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    if (id != NULL) {
        assert_int_equal(*id, ELOS_ID_INVALID);
    }
}

void elosTestElosEventSubscribeErrInvalidParameter(void **state) {
    TEST("elosEventSubscribe");
    SHOULD("%s", "return SAFU_RESULT_FAILED if one parameter is NULL");

    struct _TestState *testState = *state;
    elosSession_t *expectedSession = &testState->session;
    elosEventQueueId_t id = ELOS_ID_INVALID;
    const char *filter[] = {
        "event.source.appName,'myApp',eq",
        "event.source.appName,'linux',eq",
        "event.source.appName,'openssh',eq",
    };

    _testElosEventSubscribeErrInvalidParameterParam(NULL, NULL, 0, NULL);
    _testElosEventSubscribeErrInvalidParameterParam(NULL, filter, ARRAY_SIZE(filter), &id);
    _testElosEventSubscribeErrInvalidParameterParam(NULL, NULL, 0, &id);
    _testElosEventSubscribeErrInvalidParameterParam(expectedSession, NULL, 0, &id);
    _testElosEventSubscribeErrInvalidParameterParam(expectedSession, NULL, 0, NULL);
    _testElosEventSubscribeErrInvalidParameterParam(expectedSession, filter, ARRAY_SIZE(filter), NULL);
    _testElosEventSubscribeErrInvalidParameterParam(NULL, filter, ARRAY_SIZE(filter), NULL);
}
