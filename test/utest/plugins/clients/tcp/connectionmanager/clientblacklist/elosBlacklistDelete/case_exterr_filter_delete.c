// SPDX-License-Identifier: MIT

#include "elos/eventfilter/eventfilter.h"
#include "elosBlacklistDelete_utest.h"
#include "mock_eventfilter.h"

int elosTestElosBlacklistDeleteErrBlacklistFilterDeleteSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosBlacklistDeleteErrBlacklistFilterDeleteTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosBlacklistDeleteErrBlacklistFilterDelete(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;
    elosRpnFilterResultE_t retval = RPNFILTER_RESULT_ERROR;
    elosEventFilter_t testFilter = ELOS_EVENTFILTER_INIT;
    const char *paramstring = ".event.messageCode 2000 EQ";

    elosEventFilterParam_t param = {.filterString = paramstring};
    retval = elosEventFilterCreate(&testFilter, &param);
    assert_int_equal(retval, RPNFILTER_RESULT_OK);
    assert_int_equal(testFilter.state, RPNFILTER_FINALIZED);

    TEST("elosBlacklistDelete");
    SHOULD("%s", "not delete blacklist filter since null blacklist parameter");

    MOCK_FUNC_AFTER_CALL(elosEventFilterDeleteMembers, 0);
    expect_any(elosEventFilterDeleteMembers, filter);
    will_return(elosEventFilterDeleteMembers, RPNFILTER_RESULT_ERROR);

    result = elosBlacklistDelete(&testFilter);

    assert_int_equal(result, SAFU_RESULT_FAILED);

    elosEventFilterDeleteMembers(&testFilter);
}
