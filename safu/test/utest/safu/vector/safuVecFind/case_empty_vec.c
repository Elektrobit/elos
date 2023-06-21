// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecFind_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindEmptyVec(UNUSED void **state) {
    safuVec_t vec = {0};
    uint32_t idx;
    uint32_t val = 4;
    int result;

    safuVecCreate(&vec, 10, sizeof(uint32_t));

    result = safuVecFind(&vec, 0, safuMatchUint32, &val, &idx);
    assert_int_equal(result, 0);

    safuVecFree(&vec);

    safuVecCreate(&vec, 0, sizeof(uint32_t));

    result = safuVecFind(&vec, 0, safuMatchUint32, &val, &idx);
    assert_int_equal(result, 0);

    safuVecFree(&vec);
}
