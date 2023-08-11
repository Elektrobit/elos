// SPDX-License-Identifier: MIT
#include <sys/types.h>

#include "elosAuthorizedProcessCheck_utest.h"

int elosTestElosAuthorizedProcessCheckProcessParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessCheckProcessParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosAuthorizedProcessCheckProcessParamNull(UNUSED void **state) {
    bool result = false;
    safuVec_t testFilter = {0};

    int retValue = safuVecCreate(&testFilter, 3, sizeof(elosProcessFilter_t));
    assert_int_equal(retValue, 0);

    TEST("elosAuthorizedProcessCheck");
    SHOULD("%s", "not match process with authorized process filter since process parameter is null");

    result = elosAuthorizedProcessCheck(&testFilter, NULL);
    assert_false(result);

    safuVecFree(&testFilter);
}
