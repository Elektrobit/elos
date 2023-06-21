// SPDX-License-Identifier: MIT
#include "safuVecFree_utest.h"

void safuTestSafuVecFreeErrNull(UNUSED void **state) {
    assert_int_equal(-1, safuVecFree(NULL));
}
