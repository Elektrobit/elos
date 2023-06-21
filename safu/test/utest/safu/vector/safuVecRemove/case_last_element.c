// SPDX-License-Identifier: MIT
#include "safuVecRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecRemoveLastElement(UNUSED void **state) {
    safuVec_t vec = {0};
    const size_t initialSize = 10;
    const size_t fillSize = 5;

    safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, fillSize);

    const safuTdata_t expectedTData[5] = {{0, "str 0"}, {1, "str 1"}, {2, "str 2"}, {3, "str 3"}, {4, "str 4"}};

    int retval = safuVecRemove(&vec, 4);
    assert_int_equal(0, retval);
    assert_int_equal(4, vec.elementCount);
    for (size_t i = 0; i < 4; i++) {
        assert_true(safuMatchSafuTdata((void *)&((safuTdata_t *)vec.data)[i], (void *)&expectedTData[i]));
    }

    retval = safuVecRemove(&vec, 3);
    assert_int_equal(0, retval);
    assert_int_equal(3, vec.elementCount);
    for (size_t i = 0; i < 3; i++) {
        assert_true(safuMatchSafuTdata((void *)&((safuTdata_t *)vec.data)[i], (void *)&expectedTData[i]));
    }

    retval = safuVecRemove(&vec, 2);
    assert_int_equal(0, retval);
    assert_int_equal(2, vec.elementCount);
    for (size_t i = 0; i < 2; i++) {
        assert_true(safuMatchSafuTdata((void *)&((safuTdata_t *)vec.data)[i], (void *)&expectedTData[i]));
    }

    retval = safuVecRemove(&vec, 1);
    assert_int_equal(0, retval);
    assert_int_equal(1, vec.elementCount);
    assert_true(safuMatchSafuTdata(vec.data, (void *)expectedTData));

    retval = safuVecRemove(&vec, 0);
    assert_int_equal(0, retval);
    assert_int_equal(0, vec.elementCount);

    safuVecFree(&vec);
}
