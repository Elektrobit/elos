// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetElementNotFound(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);

    uint32_t idx = 0;
    safuTdata_t *element = NULL;
    safuTdata_t matchData = safuSafuTDataNumber(42);

    int result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, 0);
    assert_int_equal(idx, 0);
    assert_ptr_equal(element, NULL);

    idx = 5;
    safuTdata_t elmnt = {0};
    element = &elmnt;

    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, 0);
    assert_int_equal(idx, 5);
    assert_ptr_equal(element, &elmnt);

    safuVecFree(&vec);
}
