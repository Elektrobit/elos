// SPDX-License-Identifier: MIT

#include "elosRpnFilterNew_utest.h"

int elos_testElosRpnFilterNewSuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterNewSuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t init = ELOS_RPNFILTER_INIT;
    elosRpnFilter_t *ptr = NULL;

    result = elosRpnFilterNew(&ptr);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_true(ptr != NULL);
    assert_memory_equal(ptr, &init, sizeof(elosRpnFilter_t));

    result = elosRpnFilterDelete(ptr);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterNewSuccessTeardown(UNUSED void **state) {
    return 0;
}
