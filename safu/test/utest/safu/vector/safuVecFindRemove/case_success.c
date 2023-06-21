// SPDX-License-Identifier: MIT
#include "safuVecFindRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecFindRemoveSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    safuVecCreate(&vec, 20, sizeof(safuTdata_t));

    safuFillVecSafuTData(&vec, 9);

    safuTdata_t element = safuSafuTDataNumber(8);
    int result = safuVecFindRemove(&vec, safuMatchSafuTdata, &element);
    assert_int_equal(1, result);
    assert_int_equal(8, vec.elementCount);

    safuTdata_t *data = (safuTdata_t *)vec.data;
    for (int i = 0; i < 8; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }

    safuFillVecSafuTData(&vec, 5);

    element = safuSafuTDataNumber(4);
    result = safuVecFindRemove(&vec, safuMatchSafuTdata, &element);
    assert_int_equal(2, result);
    assert_int_equal(11, vec.elementCount);

    data = (safuTdata_t *)vec.data;
    for (int i = 0; i < 4; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 4; i < 7; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i + 1);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 7; i < 11; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i - 7);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }

    safuFillVecSafuTData(&vec, 3);

    element = safuSafuTDataNumber(2);
    result = safuVecFindRemove(&vec, safuMatchSafuTdata, &element);
    assert_int_equal(3, result);
    assert_int_equal(11, vec.elementCount);

    data = (safuTdata_t *)vec.data;
    for (int i = 0; i < 2; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 2; i < 3; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i + 1);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 3; i < 6; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i + 2);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 6; i < 8; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i - 6);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 8; i < 9; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i - 5);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }
    for (int i = 9; i < 11; i++) {
        safuTdata_t elem = safuSafuTDataNumber(i - 9);
        assert_true(safuMatchSafuTdata((void *)&data[i], (void *)&elem));
    }

    safuVecFree(&vec);
}
