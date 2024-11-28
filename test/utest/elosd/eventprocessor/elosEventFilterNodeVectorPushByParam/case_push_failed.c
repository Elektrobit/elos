// SPDX-License-Identifier: MIT

#include "elosEventFilterNodeVectorPushByParam_utest.h"
#include "safu/mock_log.h"
#include "safu/mock_vector.h"

#define _VECTORELEMENTS 8

int elosTestEloEventFilterNodeVectorPushByParamPushFailedSetup(void **state) {
    safuResultE_t result;
    elosTestState_t *test = safuAllocMem(NULL, sizeof(elosTestState_t));
    result = elosEventFilterNodeVectorInitialize(&test->efev, _VECTORELEMENTS);
    assert_int_equal(result, SAFU_RESULT_OK);

    *state = test;

    return 0;
}

int elosTestEloEventFilterNodeVectorPushByParamPushFailedTeardown(void **state) {
    elosTestState_t *test = *state;

    safuVecFree(&test->efev);
    free(test);

    return 0;
}

void elosTestEloEventFilterNodeVectorPushByParamPushFailed(void **state) {
    elosTestState_t *test = *state;
    safuResultE_t result;

    TEST("elosEventFilterNodeVectorInitialize");
    SHOULD("%s", "fail since elosEventFilterNodeInitialize fails");

    char const *filterStrings[] = {".event.source.appName 'test' STRCMP", "1 1 AND"};
    elosEventFilterNodeParam_t const param = {.filterStrings = filterStrings, .filterStringCount = 2};

    MOCK_FUNC_AFTER_CALL(safuVecPush, 18);
    MOCK_FUNC_ENABLE(safuLogFunc);

    expect_any(__wrap_safuVecPush, vec);
    expect_any(__wrap_safuVecPush, element);
    will_return(__wrap_safuVecPush, -1);

    expect_value(__wrap_safuLogFunc, level, SAFU_LOG_LEVEL_ERR);
    expect_any(__wrap_safuLogFunc, file);
    expect_any(__wrap_safuLogFunc, func);
    expect_any(__wrap_safuLogFunc, line);
    expect_string(__wrap_safuLogFunc, message, "safuVecPush failed");
    will_return(__wrap_safuLogFunc, SAFU_RESULT_OK);

    result = elosEventFilterNodeVectorPushByParam(&test->efev, &param);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
