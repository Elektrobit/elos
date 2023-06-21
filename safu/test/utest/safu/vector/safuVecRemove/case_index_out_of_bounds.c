// SPDX-License-Identifier: MIT
#include "safuVecRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecRemoveErrIndexOutOfBounds(UNUSED void **state) {
    safuVec_t vec = {0};
    const size_t initialSize = 10;
    const size_t fillSize = 5;

    safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));

    assert_int_equal(-1, safuVecRemove(&vec, 0));
    assert_int_equal(-1, safuVecRemove(&vec, 3));

    safuFillVecSafuTData(&vec, fillSize);

    assert_int_equal(-1, safuVecRemove(&vec, fillSize));
    assert_int_equal(-1, safuVecRemove(&vec, fillSize + 2));
    assert_int_equal(-1, safuVecRemove(&vec, initialSize));
    assert_int_equal(-1, safuVecRemove(&vec, initialSize + 2));

    safuVecFree(&vec);
}
