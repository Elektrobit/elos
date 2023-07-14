// SPDX-License-Identifier: MIT

#include "elosRpnFilterClone_utest.h"

int elos_testelosRpnFilterCloneSuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testelosRpnFilterCloneSuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t *to = NULL;
    elosRpnFilter_t from = {0};

    result = elosRpnFilterClone(&to, &from);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_non_null(to);

    result = elosRpnFilterDelete(to);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testelosRpnFilterCloneSuccessTeardown(UNUSED void **state) {
    return 0;
}
