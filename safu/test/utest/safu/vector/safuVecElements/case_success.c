// SPDX-License-Identifier: MIT
#include "safuVecElements_utest.h"
#include "vector_helper.h"

void safuTestSafuVecElementsSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    safuVecCreate(&vec, 20, sizeof(safuTdata_t));

    assert_int_equal(0, safuVecElements(&vec));

    safuFillVecSafuTData(&vec, 10);

    assert_int_equal(10, safuVecElements(&vec));

    safuVecFree(&vec);
}
