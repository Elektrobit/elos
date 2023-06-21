// SPDX-License-Identifier: MIT

#include "elosRpnFilterDeleteMembers_utest.h"

int elos_testElosRpnFilterDeleteMembersSuccessSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterDeleteMembersSuccess(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t *test = NULL;

    result = elosRpnFilterNew(&test);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_true(test != NULL);

    result = elosRpnFilterDeleteMembers(test);
    assert_true(result == RPNFILTER_RESULT_OK);
    assert_null(test->state);
    assert_null(test->steps);

    result = elosRpnFilterDelete(test);
    assert_true(result == RPNFILTER_RESULT_OK);
}

int elos_testElosRpnFilterDeleteMembersSuccessTeardown(UNUSED void **state) {
    return 0;
}
