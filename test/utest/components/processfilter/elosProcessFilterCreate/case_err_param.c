// SPDX-License-Identifier: MIT

#include "elosProcessFilterCreate_utest.h"

int elosTestEloProcessFilterCreateErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloProcessFilterCreateErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosProcessFilter_t filter = ELOS_PROCESSFILTER_INIT;
    elosProcessFilterParam_t param = {.filterString = "1 .1 EQ"};

    TEST("elosProcessFilterCreate");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosProcessFilterCreate(NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "NULL, &param");
    result = elosProcessFilterCreate(NULL, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, NULL");
    result = elosProcessFilterCreate(&filter, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=NULL");
    param.filterString = NULL;
    result = elosProcessFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=\"\"");
    param.filterString = "";
    result = elosProcessFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloProcessFilterCreateErrParamTeardown(UNUSED void **state) {
    return 0;
}
