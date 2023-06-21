// SPDX-License-Identifier: MIT

#include "elosRpnFilterDelete_utest.h"

int elos_testElosRpnFilterDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterDeleteSuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t *test = NULL;

    result = elosRpnFilterNew(&test);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_true(test != NULL);

    result = elosRpnFilterDelete(test);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}
