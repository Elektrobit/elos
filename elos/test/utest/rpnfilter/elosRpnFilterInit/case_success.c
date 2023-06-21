// SPDX-License-Identifier: MIT

#include "elosRpnFilterInit_utest.h"

int elos_testElosRpnFilterInitSuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterInitSuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t compareValue = ELOS_RPNFILTER_INIT;
    elosRpnFilter_t testValue;

    result = elosRpnFilterInit(&testValue);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_memory_equal(&testValue, &compareValue, sizeof(elosRpnFilter_t));
}

int elos_testElosRpnFilterInitSuccessTeardown(UNUSED void **state) {
    return 0;
}
