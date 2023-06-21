// SPDX-License-Identifier: MIT
#include "safuVecGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecGetSuccess(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 5);

    for (uint32_t i = 0; i < 5; i++) {
        safuTdata_t *result = (safuTdata_t *)safuVecGet(&vec, i);
        safuTdata_t data = safuSafuTDataNumber(i);
        assert_true(safuMatchSafuTdata(result, &data));
    }
    safuVecFree(&vec);
}
