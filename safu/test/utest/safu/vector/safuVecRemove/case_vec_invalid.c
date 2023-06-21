// SPDX-License-Identifier: MIT
#include "safuVecRemove_utest.h"

void safuTestSafuVecRemoveErrVecInvalid(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_int_equal(-1, safuVecRemove(NULL, 0));
    assert_int_equal(-1, safuVecRemove(NULL, 3));

    assert_int_equal(-1, safuVecRemove(&vec, 0));
    assert_int_equal(-1, safuVecRemove(&vec, 3));

    vec.elementCount = 10;

    assert_int_equal(-1, safuVecRemove(&vec, 0));
    assert_int_equal(-1, safuVecRemove(&vec, 3));
}
