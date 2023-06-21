// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetErrNoMatchData(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);

    uint32_t idx = 0;
    safuTdata_t *element = NULL;

    int result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, NULL);

    assert_int_equal(result, 0);
    assert_int_equal(idx, 0);
    assert_ptr_equal(element, NULL);

    idx = 12;
    safuTdata_t elmnt = {0};
    element = &elmnt;

    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, NULL);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &elmnt);

    result = safuVecFindGet(&vec, NULL, &idx, safuMatchSafuTdata, NULL);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);

    safuVecFree(&vec);
}
