// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elos/libelos/libelos.h"
#include "elosGetVersion_utest.h"
#include "safu/mock_safu.h"

int elosTestElosGetVersionErrCallSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosGetVersionErrCallTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosGetVersionErrCall(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    char const *versionStr = NULL;

    TEST("elosGetVersion");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "Sending message fails");
    elosMockSendMessage(test, SAFU_RESULT_FAILED);
    result = elosGetVersion(&test->session, &versionStr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(versionStr);

    PARAM("%s", "Receiving message fails");
    elosMockSendMessage(test, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, NULL, SAFU_RESULT_FAILED);
    result = elosGetVersion(&test->session, &versionStr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(versionStr);

    PARAM("%s", "Reading version field fails");
    elosMockSendMessage(test, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, test->emptyResponseStr, SAFU_RESULT_OK);
    result = elosGetVersion(&test->session, &versionStr);
    assert_int_equal(result, SAFU_RESULT_FAILED);
    assert_null(versionStr);
}
