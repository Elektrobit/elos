// SPDX-License-Identifier: MIT
#include "safuVecFindGet_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindGetSuccess(UNUSED void **state) {
    safuVec_t vec = {0};

    safuVecCreate(&vec, 20, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, 10);
    safuFillVecSafuTData(&vec, 5);
    safuFillVecSafuTData(&vec, 5);

    uint32_t idx = 0;
    safuTdata_t *element = NULL;
    safuTdata_t matchData = safuSafuTDataNumber(2);

    PARAM("%s", "Search with idx parameter");
    int result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);
    assert_int_equal(result, 1);
    assert_int_equal(idx, 2);
    assert_ptr_equal(element, &((safuTdata_t *)vec.data)[idx]);
    assert_int_equal(element->val, matchData.val);
    assert_string_equal(element->str, matchData.str);

    idx = 6;
    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);
    assert_int_equal(result, 1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &((safuTdata_t *)vec.data)[idx]);
    assert_int_equal(element->val, matchData.val);
    assert_string_equal(element->str, matchData.str);

    idx = 12;
    result = safuVecFindGet(&vec, (void **)&element, &idx, safuMatchSafuTdata, &matchData);
    assert_int_equal(result, 1);
    assert_int_equal(idx, 12);
    assert_ptr_equal(element, &((safuTdata_t *)vec.data)[idx]);
    assert_int_equal(element->val, matchData.val);
    assert_string_equal(element->str, matchData.str);

    PARAM("%s", "Search without idx parameter");
    result = safuVecFindGet(&vec, (void **)&element, NULL, safuMatchSafuTdata, &matchData);
    assert_int_equal(result, 1);
    assert_ptr_equal(element, &((safuTdata_t *)vec.data)[2]);
    assert_int_equal(element->val, matchData.val);
    assert_string_equal(element->str, matchData.str);

    safuVecFree(&vec);
}
