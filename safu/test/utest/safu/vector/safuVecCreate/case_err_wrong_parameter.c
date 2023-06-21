// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuVecCreate_utest.h"

void safuTestSafuVecCreateWrongParameter(UNUSED void **state) {
    safuVec_t vec = {0};

    TEST("safuVecCreate");
    SHOULD("%s", "return an error code due to wrong parameters");

    int retval = safuVecCreate(&vec, 10, 0);
    assert_int_equal(retval, -1);

    retval = safuVecCreate(NULL, 10, sizeof(int32_t));
    assert_int_equal(retval, -1);

    retval = safuVecCreate(&vec, 0, 0);
    assert_int_equal(retval, -1);

    retval = safuVecCreate(NULL, 10, 0);
    assert_int_equal(retval, -1);

    retval = safuVecCreate(NULL, 0, sizeof(int32_t));
    assert_int_equal(retval, -1);

    retval = safuVecCreate(NULL, 0, 0);
    assert_int_equal(retval, -1);
}
