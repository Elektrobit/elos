// SPDX-License-Identifier: MIT
#include "elosEventDelete_utest.h"

int elosTestElosEventDeleteWrongParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeleteWrongParameterTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeleteWrongParameter(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventDelete");
    SHOULD("%s", "return failure if parameter is null");

    result = elosEventDelete(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
