// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elos/libelos/libelos.h"
#include "elosGetVersion_utest.h"
#include "safu/mock_safu.h"

int elosTestElosGetVersionSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosGetVersionSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosGetVersionSuccess(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;
    char const *versionStr = NULL;

    TEST("elosGetVersion");
    SHOULD("%s", "successfully receive a version string");

    elosMockSendMessage(test, SAFU_RESULT_OK);
    elosMockReceiveJsonMessage(test, test->normalResponseStr, SAFU_RESULT_OK);
    result = elosGetVersion(&test->session, &versionStr);
    assert_int_equal(result, SAFU_RESULT_OK);
    assert_non_null(versionStr);
    assert_string_equal(versionStr, MOCK_VERSIONSTR);
}
