// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetErrUninitializedVec(UNUSED void **state) {
    safuVec_t vec = {0};
    vec.elementCount = 10;  // fake element count

    uint32_t idx = 0;
    safuTdata_t *element = NULL;
    safuTdata_t matchData = safuSafuTDataNumber(42);

    int result = safuVecFindGet(NULL, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 0);
    assert_ptr_equal(element, NULL);

    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 0);
    assert_ptr_equal(element, NULL);

    idx = 12;
    safuTdata_t elmnt = {0};
    element = &elmnt;

    result = safuVecFindGet(NULL, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &elmnt);

    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &elmnt);

    safuVecFree(&vec);
}
