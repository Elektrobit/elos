// SPDX-License-Identifier: MIT

#include "elosRpnFilterDeepCopy_utest.h"

int elos_testElosRpnFilterDeepCopySuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterDeepCopySuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t to = {0};
    elosRpnFilter_t from = {0};

    result = elosRpnFilterDeepCopy(&to, &from);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterDeepCopySuccessTeardown(UNUSED void **state) {
    return 0;
}
