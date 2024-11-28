// SPDX-License-Identifier: MIT

#include "elosEventFilterManagerInitialize_utest.h"

#define _VECTORELEMENTS 1

int elosTestEloEventFilterManagerInitializeErrParamSetup(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    safuResultE_t result;

    elosEventFilterManagerParam_t const param = {
        .elements = _VECTORELEMENTS,
        .eventQueueManager = &test->eqm,
        .idManager = &test->idm,
    };
    result = elosEventFilterManagerInitialize(&test->efm, &param);
    assert_int_equal(result, SAFU_RESULT_OK);

    return 0;
}

int elosTestEloEventFilterManagerInitializeErrParamTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventFilterManagerDeleteMembers(&test->efm);
    return 0;
}

void elosTestEloEventFilterManagerInitializeErrParam(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;
    elosEventFilterManager_t eventFilterManager = {0};
    safuResultE_t result;
    elosEventFilterManagerParam_t const param = {
        .elements = _VECTORELEMENTS,
        .eventQueueManager = &test->eqm,
        .idManager = &test->idm,
    };

    TEST("elosEventFilterManagerInitialize");
    SHOULD("%s", "test correct behaviour with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventFilterManagerInitialize(NULL, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "NULL, param");
    result = elosEventFilterManagerInitialize(NULL, &param);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "eventFilterManager, NULL");
    result = elosEventFilterManagerInitialize(&eventFilterManager, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "initializedEventFilterManager, NULL");
    result = elosEventFilterManagerInitialize(&test->efm, NULL);
    assert_true(result == SAFU_RESULT_FAILED);

    PARAM("%s", "initializedEventFilterManager, param");
    result = elosEventFilterManagerInitialize(&test->efm, &param);
    assert_true(result == SAFU_RESULT_FAILED);
}
