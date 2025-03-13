// SPDX-License-Identifier: MIT

#include <unix_clientauthorization/clientauthorization.h>

#include "elosUnixClientAuthorizationDelete_utest.h"

int elosTestelosUnixClientAuthorizationDeleteSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestelosUnixClientAuthorizationDeleteSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestelosUnixClientAuthorizationDeleteSuccess(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosUnixClientAuthorizationDelete");
    SHOULD("%s", "call elosAuthorizedProcessDelete to delete authorizedProcessFilters");

    elosClientAuthorization_t clientAuth = {0};
    clientAuth.authorizedProcessFilters.elementSize = 1;

    MOCK_FUNC_AFTER_CALL(elosAuthorizedProcessDelete, 0);
    expect_value(elosAuthorizedProcessDelete, authorizedprocesses, &clientAuth.authorizedProcessFilters);
    will_return(elosAuthorizedProcessDelete, SAFU_RESULT_OK);

    result = elosUnixClientAuthorizationDelete(&clientAuth);
    assert_int_equal(result, SAFU_RESULT_OK);
}
