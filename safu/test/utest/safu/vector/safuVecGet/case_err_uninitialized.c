// SPDX-License-Identifier: MIT
#include "safuVecGet_utest.h"

void safuTestSafuVecGetErrUninitialized(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_null(safuVecGet(&vec, 0));
    assert_null(safuVecGet(&vec, 5));
    assert_null(safuVecGet(&vec, 15));
}
