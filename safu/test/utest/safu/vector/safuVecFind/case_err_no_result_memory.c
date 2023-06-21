// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecFind_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindErrNoResultMemory(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(uint32_t));

    safuFillVecUint32(&vec, 6);

    uint32_t val = 4;

    int result = safuVecFind(&vec, 0, safuMatchUint32, &val, NULL);
    assert_int_equal(result, 1);

    safuVecFree(&vec);
}
