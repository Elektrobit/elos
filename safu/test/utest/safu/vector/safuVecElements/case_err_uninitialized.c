// SPDX-License-Identifier: MIT
#include "safuVecElements_utest.h"

void safuTestSafuVecElementsErrUninitialized(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_int_equal(0, safuVecElements(&vec));
}
