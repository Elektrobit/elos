// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecFind_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindElementNotFound(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(uint32_t));

    uint32_t idx;
    uint32_t val = 50;

    safuFillVecUint32(&vec, 4);

    int result = safuVecFind(&vec, 0, safuMatchUint32, &val, &idx);
    assert_int_equal(result, 0);

    safuVecFree(&vec);
}
