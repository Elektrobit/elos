// SPDX-License-Identifier: MIT

#include "elos/libelos/libelos.h"
#include "elosSessionValid_utest.h"

int elosTestElosSessionValidSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosSessionValidSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosSessionValidSuccess(void **state) {
    elosSession_t *session = *state;
    elosSession_t invalidSession = {0};
    bool result;

    TEST("elosSessionValid");
    SHOULD("%s", "test correct behaviour of elosSessionValid");

    result = elosSessionValid(session);
    assert_int_equal(result, true);

    result = elosSessionValid(&invalidSession);
    assert_int_equal(result, false);

    result = elosSessionValid(NULL);
    assert_int_equal(result, false);
}
