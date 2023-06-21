// SPDX-License-Identifier: MIT
#include "safuVecGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecGetErrOutOfBounds(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 5);

    assert_null(safuVecGet(&vec, 5));
    assert_null(safuVecGet(&vec, 15));

    safuVecFree(&vec);
}
