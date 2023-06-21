// SPDX-License-Identifier: MIT
#include "safuVecFind_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindErrNoDataVec(UNUSED void **state) {
    safuVec_t vec = {0};
    uint32_t val = 1;
    uint32_t idx = 1;

    vec.elementCount++;  // fake element count

    int result = safuVecFind(&vec, 0, safuMatchUint32, &val, &idx);
    assert_int_equal(result, -1);

    result = safuVecFind(&vec, 0, safuMatchUint32, NULL, &idx);
    assert_int_equal(result, -1);

    result = safuVecFind(&vec, 0, safuMatchUint32, &val, NULL);
    assert_int_equal(result, -1);

    result = safuVecFind(&vec, 0, NULL, &val, &idx);
    assert_int_equal(result, -1);

    safuVecFree(&vec);
}
