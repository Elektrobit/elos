// SPDX-License-Identifier: MIT

#include "elosRpnFilterCreate_utest.h"

int elos_testElosRpnFilterCreateErrParamSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterCreateErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t filter = ELOS_RPNFILTER_INIT;
    elosRpnFilterParam_t param = {.filterString = "1 .1 EQ"};

    TEST("elosRpnFilterCreate");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosRpnFilterCreate(NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "NULL, &param");
    result = elosRpnFilterCreate(NULL, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, NULL");
    result = elosRpnFilterCreate(&filter, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=NULL");
    param.filterString = NULL;
    result = elosRpnFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=\"\"");
    param.filterString = "";
    result = elosRpnFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elos_testElosRpnFilterCreateErrParamTeardown(UNUSED void **state) {
    return 0;
}
