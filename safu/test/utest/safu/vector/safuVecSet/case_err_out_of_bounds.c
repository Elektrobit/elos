// SPDX-License-Identifier: MIT
#include "safuVecSet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecSetErrOutOfBounds(UNUSED void **state) {
    safuVec_t vec = {0};
    safuTdata_t element = safuSafuTDataNumber(42);

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    assert_int_equal(-1, safuVecSet(&vec, 0, &element));
    assert_int_equal(-1, safuVecSet(&vec, 5, &element));

    safuFillVecSafuTData(&vec, 5);

    assert_int_equal(-1, safuVecSet(&vec, 10, &element));

    safuVecFree(&vec);
}
