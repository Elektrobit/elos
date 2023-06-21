// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecFindRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindRemoveErrNoMatchFunc(UNUSED void **state) {
    safuVec_t vec = {0};
    safuVecCreate(&vec, 20, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);
    safuTdata_t element = safuSafuTDataNumber(3);

    assert_int_equal(-1, safuVecFindRemove(&vec, NULL, &element));
    assert_int_equal(-1, safuVecFindRemove(&vec, NULL, NULL));

    safuVecFree(&vec);
}
