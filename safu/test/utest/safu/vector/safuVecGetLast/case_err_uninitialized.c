// SPDX-License-Identifier: MIT
#include "safuVecGetLast_utest.h"

void safuTestSafuVecGetLastErrUninitialized(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_null(safuVecGetLast(NULL));
    assert_null(safuVecGetLast(&vec));
}
