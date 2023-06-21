// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_libc.h"
#include "safuVecPush_utest.h"

void safuTestSafuVecPushWithoutResize(UNUSED void **state) {
    safuVec_t vec = {0};
    safuTdata_t tdata[10] = {0};
    safuTdata_t expectedTdata[10] = {
        {420, "420"}, {421, "421"}, {422, "422"}, {423, "423"}, {424, "424"},
        {425, "425"}, {426, "426"}, {427, "427"}, {428, "428"}, {429, "429"},
    };

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, sizeof(tdata));
    will_return(__wrap_malloc, &tdata);

    int retval = safuVecCreate(&vec, ARRAY_SIZE(tdata), sizeof(safuTdata_t));
    assert_int_equal(retval, 0);

    for (size_t i = 0; i < ARRAY_SIZE(tdata); i++) {
        retval = safuVecPush(&vec, &expectedTdata[i]);
        assert_int_equal(retval, 0);
        assert_memory_equal(tdata, expectedTdata, sizeof(safuTdata_t) * (i + 1));
    }
}
