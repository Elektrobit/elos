// SPDX-License-Identifier: MIT
#include "safuVecPop_utest.h"
#include "vector_helper.h"

void safuTestSafuVecPopEmpty(UNUSED void **state) {
    safuVec_t vec = {0};

    uint32_t capacity = 10;
    safuVecCreate(&vec, capacity, sizeof(safuTdata_t));

    // pop one more time than the number of allocated elements
    for (uint32_t i = 0; i < capacity + 1; i++) {
        int result = safuVecPop(&vec);

        assert_int_equal(0, result);
        assert_int_equal(0, vec.elementCount);
    }
    safuVecFree(&vec);
}
