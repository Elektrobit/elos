// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetErrNoResultMemory(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 10, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);

    uint32_t idx = 0;
    safuTdata_t matchData = safuSafuTDataNumber(3);

    int result = safuVecFindGet(&vec, NULL, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 0);

    idx = 2;

    result = safuVecFindGet(&vec, NULL, &idx, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 2);

    result = safuVecFindGet(&vec, NULL, NULL, safuMatchSafuTdata, &matchData);

    assert_int_equal(result, -1);

    result = safuVecFindGet(&vec, NULL, &idx, safuMatchSafuTdata, NULL);

    assert_int_equal(result, -1);
    assert_int_equal(idx, 2);

    result = safuVecFindGet(&vec, NULL, NULL, safuMatchSafuTdata, NULL);

    assert_int_equal(result, -1);

    safuVecFree(&vec);
}
