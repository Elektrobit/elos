// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "elos/libelos/libelos.h"
#include "elosGetVersion_utest.h"
#include "safu/mock_safu.h"

int elosTestElosGetVersionErrParamSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosGetVersionErrParamTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosGetVersionErrParam(UNUSED void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosSession_t invalidSession = {0};
    safuResultE_t result;
    char const *versionStr = NULL;

    TEST("elosGetVersion");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosGetVersion(NULL, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, &versionStr");
    result = elosGetVersion(NULL, &versionStr);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, NULL");
    result = elosGetVersion(&invalidSession, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&invalidSession, &versionStr");
    result = elosGetVersion(&invalidSession, &versionStr);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("%s", "&session, NULL");
    result = elosGetVersion(&test->session, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
