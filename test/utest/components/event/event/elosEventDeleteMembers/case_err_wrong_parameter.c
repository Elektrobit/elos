// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "cmocka_mocks/mock_libc.h"
#include "elosEventDeleteMembers_utest.h"

int elosTestElosEventDeleteMembersWrongParameterSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosEventDeleteMembersWrongParameterTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosEventDeleteMembersWrongParameter(UNUSED void **state) {
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("elosEventDeleteMembers");
    SHOULD("%s", "return failure if parameter is null");

    result = elosEventDeleteMembers(NULL);

    assert_int_equal(result, SAFU_RESULT_FAILED);
}
