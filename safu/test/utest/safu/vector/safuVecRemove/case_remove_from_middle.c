// SPDX-License-Identifier: MIT
#include "safuVecRemove_utest.h"
#include "vector_helper.h"

void safuTestSafuVecRemoveFromMiddle(UNUSED void **state) {
    safuVec_t vec = {0};
    const size_t initialSize = 10;
    size_t fillSize = 10;

    safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));
    safuFillVecSafuTData(&vec, fillSize);

    {
        int retval = safuVecRemove(&vec, 0);
        fillSize -= 1;
        assert_int_equal(0, retval);
        assert_int_equal(fillSize, vec.elementCount);
        safuTdata_t expectedTData[9] = {{1, "str 1"}, {2, "str 2"}, {3, "str 3"}, {4, "str 4"}, {5, "str 5"},
                                        {6, "str 6"}, {7, "str 7"}, {8, "str 8"}, {9, "str 9"}};
        for (size_t i = 0; i < fillSize; i++) {
            assert_true(
                safuMatchSafuTdata((const void *)&((safuTdata_t *)vec.data)[i], (const void *)&expectedTData[i]));
        }
    }

    {
        int retval = safuVecRemove(&vec, 2);
        fillSize -= 1;
        assert_int_equal(0, retval);
        assert_int_equal(fillSize, vec.elementCount);
        safuTdata_t expectedTData[8] = {{1, "str 1"}, {2, "str 2"}, {4, "str 4"}, {5, "str 5"},
                                        {6, "str 6"}, {7, "str 7"}, {8, "str 8"}, {9, "str 9"}};
        for (size_t i = 0; i < fillSize; i++) {
            assert_true(
                safuMatchSafuTdata((const void *)&((safuTdata_t *)vec.data)[i], (const void *)&expectedTData[i]));
        }
    }

    {
        int retval = safuVecRemove(&vec, 3);
        fillSize -= 1;
        assert_int_equal(0, retval);
        assert_int_equal(fillSize, vec.elementCount);
        safuTdata_t expectedTData[7] = {{1, "str 1"}, {2, "str 2"}, {4, "str 4"}, {6, "str 6"},
                                        {7, "str 7"}, {8, "str 8"}, {9, "str 9"}};
        for (size_t i = 0; i < fillSize; i++) {
            assert_true(
                safuMatchSafuTdata((const void *)&((safuTdata_t *)vec.data)[i], (const void *)&expectedTData[i]));
        }
    }

    safuVecFree(&vec);
}
