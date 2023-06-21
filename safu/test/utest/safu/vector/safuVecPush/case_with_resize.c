// SPDX-License-Identifier: MIT
#include <cmocka_extensions/cmocka_extensions.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuVecPush_utest.h"

void safuTestSafuVecPushWithResize(UNUSED void **state) {
    safuTdata_t tdata[10] = {0};
    safuTdata_t expectedTdata[ARRAY_SIZE(tdata)] = {
        {420, "420"}, {421, "421"}, {422, "422"}, {423, "423"}, {424, "424"},
        {425, "425"}, {426, "426"}, {427, "427"}, {428, "428"}, {429, "429"},
    };
    size_t initialSize = ARRAY_SIZE(tdata) / 2;
    safuVec_t vec = {0};

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, sizeof(safuTdata_t) * initialSize);
    will_return(__wrap_malloc, &tdata);

    int retval = safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));
    assert_int_equal(retval, 0);

    for (size_t i = 0; i < initialSize; i++) {
        retval = safuVecPush(&vec, &expectedTdata[i]);
        assert_int_equal(retval, 0);
        assert_memory_equal(tdata, expectedTdata, sizeof(safuTdata_t) * (i + 1));
    }

    for (size_t i = initialSize; i < ARRAY_SIZE(tdata); i++) {
        MOCK_FUNC_AFTER_CALL(realloc, 0);
        expect_value(__wrap_realloc, ptr, tdata);
        expect_value(__wrap_realloc, newSize, sizeof(safuTdata_t) * (i + 1));
        will_return(__wrap_realloc, tdata);

        retval = safuVecPush(&vec, &expectedTdata[i]);
        assert_int_equal(retval, 0);
        assert_memory_equal(tdata, expectedTdata, sizeof(safuTdata_t) * (i + 1));
    }
}
