// SPDX-License-Identifier: MIT
#include "safuVecFree_utest.h"

void safuTestSafuVecFreeUninitialized(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_int_equal(0, safuVecFree(&vec));
}
