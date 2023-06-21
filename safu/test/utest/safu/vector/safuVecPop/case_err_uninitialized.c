// SPDX-License-Identifier: MIT
#include "safuVecPop_utest.h"
#include "vector_helper.h"

void safuTestSafuVecPopErrUninitialized(UNUSED void **state) {
    safuVec_t vec = {0};

    assert_int_equal(-1, safuVecPop(&vec));
}
