// SPDX-License-Identifier: MIT
#include "safuVecGetLast_utest.h"

void safuTestSafuVecGetLastErrEmpty(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    assert_null(safuVecGetLast(&vec));

    safuVecFree(&vec);
}
