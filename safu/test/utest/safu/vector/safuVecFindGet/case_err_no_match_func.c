// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetErrNoMatchFunc(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);

    uint32_t idx = 0;
    safuTdata_t *element = NULL;
    safuTdata_t matchData = safuSafuTDataNumber(2);

    int result = safuVecFindGet(&vec, (void **)&element, &idx, NULL, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 0);
    assert_ptr_equal(element, NULL);

    idx = 12;
    safuTdata_t elmnt = {0};
    element = &elmnt;

    result = safuVecFindGet(&vec, (void **)&element, &idx, NULL, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &elmnt);

    result = safuVecFindGet(&vec, (void **)&element, &idx, NULL, NULL);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &elmnt);

    safuVecFree(&vec);
}
