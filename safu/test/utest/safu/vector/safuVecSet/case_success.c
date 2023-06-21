// SPDX-License-Identifier: MIT
#include "safuVecSet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecSetSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    safuTdata_t element = safuSafuTDataNumber(42);

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 5);

    assert_int_equal(0, safuVecSet(&vec, 0, &element));
    assert_true(safuMatchSafuTdata(safuVecGet(&vec, 0), &element));

    assert_int_equal(0, safuVecSet(&vec, 3, &element));
    assert_true(safuMatchSafuTdata(safuVecGet(&vec, 3), &element));

    safuVecFree(&vec);
}
