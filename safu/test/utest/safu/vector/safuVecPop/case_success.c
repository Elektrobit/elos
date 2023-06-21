// SPDX-License-Identifier: MIT
#include "safuVecPop_utest.h"
#include "vector_helper.h"

void safuTestSafuVecPopSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    uint32_t count = 10;

    safuVecCreate(&vec, count, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, count);

    for (; count > 0; count--) {
        int result = safuVecPop(&vec);

        assert_int_equal(0, result);
        assert_int_equal(count - 1, vec.elementCount);
    }
    safuVecFree(&vec);
}
