// SPDX-License-Identifier: MIT
#include "safuVecFree_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFreeSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    safuVecCreate(&vec, 20, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 10);

    assert_int_equal(0, safuVecFree(&vec));
    assert_int_equal(0, vec.elementCount);
    assert_int_equal(0, vec.elementSize);
    assert_int_equal(0, vec.memorySize);
    assert_ptr_equal(NULL, vec.data);
}
