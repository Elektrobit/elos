// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecGetLast_utest.h"
#include "vector_helper.h"

void safuTestSafuVecGetLastSuccess(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 5);

    safuTdata_t *last = (safuTdata_t *)safuVecGetLast(&vec);
    safuTdata_t data = safuSafuTDataNumber(4);

    assert_true(safuMatchSafuTdata(last, &data));

    safuVecFree(&vec);
}
