// SPDX-License-Identifier: MIT

#include "elosEventFilterCreate_utest.h"

int elosTestEloEventFilterCreateErrParamSetup(UNUSED void **state) {
    return 0;
}

void elosTestEloEventFilterCreateErrParam(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosEventFilter_t filter = ELOS_EVENTFILTER_INIT;
    elosEventFilterParam_t param = {.filterString = "1 .1 EQ"};

    TEST("elosEventFilterCreate");
    SHOULD("%s", "return SAFU_RESULT_FAILED with invalid parameters");

    PARAM("%s", "NULL, NULL");
    result = elosEventFilterCreate(NULL, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "NULL, &param");
    result = elosEventFilterCreate(NULL, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, NULL");
    result = elosEventFilterCreate(&filter, NULL);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=NULL");
    param.filterString = NULL;
    result = elosEventFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);

    PARAM("%s", "&filter, &param.filterString=\"\"");
    param.filterString = "";
    result = elosEventFilterCreate(&filter, &param);
    assert_true(result == RPNFILTER_RESULT_ERROR);
}

int elosTestEloEventFilterCreateErrParamTeardown(UNUSED void **state) {
    return 0;
}
