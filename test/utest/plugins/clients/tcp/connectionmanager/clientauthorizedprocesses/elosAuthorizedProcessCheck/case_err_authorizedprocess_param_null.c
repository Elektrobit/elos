// SPDX-License-Identifier: MIT
#include <clientauthorizedprocesses/clientauthorizedprocesses.h>
#include <elos/common/process.h>

#include "elosAuthorizedProcessCheck_utest.h"

int elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNullSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNullTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosAuthorizedProcessCheckAuthorizedprocessParamNull(UNUSED void **state) {
    bool result = false;
    elosProcessIdentity_t mockProcess = {.uid = 123, .gid = 456, .pid = 789, .exec = "/bin/crinit"};

    TEST("elosAuthorizedProcessCheck");
    SHOULD("%s", "not match process with authorized process filter since authorized process parameter is null ");

    result = elosAuthorizedProcessCheck(NULL, &mockProcess);

    assert_false(result);
}
