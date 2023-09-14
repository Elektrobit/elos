// SPDX-License-Identifier: MIT

#include "elosRpnFilterExecuteResult_utest.h"

int elos_testElosRpnFilterExecuteResultSuccessMatchTrueSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterExecuteResultSuccessMatchTrue(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t init = ELOS_RPNFILTER_INIT;
    const elosRpnFilterParam_t param = {.filterString = "1 1 EQ"};
    const elosRpnFilterStack_t input = {.count = 0};
    elosRpnFilterStack_t output;

    result = elosRpnFilterCreate(&init, &param);
    assert_true(result == RPNFILTER_RESULT_OK);

    result = elosRpnFilterExecuteResult(&init, &input, &output);
    assert_true(result == RPNFILTER_RESULT_OK);

    result = elosRpnFilterDeleteMembers(&init);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterExecuteResultSuccessMatchTrueTeardown(UNUSED void **state) {
    return 0;
}
