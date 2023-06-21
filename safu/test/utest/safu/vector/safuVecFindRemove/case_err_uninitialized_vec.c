// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "safuVecFindRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindRemoveErrUninitializedVec(UNUSED void **state) {
    safuVec_t vec = {0};
    safuTdata_t element = safuSafuTDataNumber(42);

    assert_int_equal(-1, safuVecFindRemove(NULL, safuMatchSafuTdata, &element));
    assert_int_equal(-1, safuVecFindRemove(NULL, NULL, &element));
    assert_int_equal(-1, safuVecFindRemove(NULL, safuMatchSafuTdata, NULL));
    assert_int_equal(-1, safuVecFindRemove(NULL, NULL, NULL));
    assert_int_equal(-1, safuVecFindRemove(&vec, safuMatchSafuTdata, &element));
    assert_int_equal(-1, safuVecFindRemove(&vec, NULL, &element));
    assert_int_equal(-1, safuVecFindRemove(&vec, safuMatchSafuTdata, NULL));
    assert_int_equal(-1, safuVecFindRemove(&vec, NULL, NULL));
}
