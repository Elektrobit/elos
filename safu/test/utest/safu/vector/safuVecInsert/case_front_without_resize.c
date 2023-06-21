// SPDX-License-Identifier: MIT
#include <stddef.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuVecInsert_utest.h"

void safuTestSafuVecInsertFrontWithoutResize(UNUSED void **state) {
    safuTdata_t testInput[10] = {
        {420, "420"}, {421, "421"}, {422, "422"}, {423, "423"}, {424, "424"},
        {425, "425"}, {426, "426"}, {427, "427"}, {428, "428"}, {429, "429"},
    };
    safuTdata_t tdata[10] = {0};
    const size_t initialSize = ARRAY_SIZE(tdata);
    safuVec_t vec = {0};

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, initialSize * sizeof(safuTdata_t));
    will_return(__wrap_malloc, &tdata);

    int retval = safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));
    assert_int_equal(retval, 0);

    for (size_t i = 0; i < ARRAY_SIZE(tdata); i++) {
        retval = safuVecInsert(&vec, 0, &testInput[i]);
        assert_int_equal(retval, 0);
        assert_memory_equal(&tdata[0], &testInput[i], sizeof(safuTdata_t));
    }

    safuTdata_t expectedTdata[10] = {
        {429, "429"}, {428, "428"}, {427, "427"}, {426, "426"}, {425, "425"},
        {424, "424"}, {423, "423"}, {422, "422"}, {421, "421"}, {420, "420"},
    };
    assert_memory_equal(tdata, expectedTdata, sizeof(expectedTdata));
}
