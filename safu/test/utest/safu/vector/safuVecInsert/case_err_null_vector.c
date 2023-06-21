// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_libc.h"
#include "safuVecInsert_utest.h"

void safuTestSafuVecInsertErrNullVector(UNUSED void **state) {
    safuTdata_t testInput = {420, "420"};
    assert_int_equal(-1, safuVecInsert(NULL, 0, &testInput));
    assert_int_equal(-1, safuVecInsert(NULL, 1, &testInput));
    assert_int_equal(-1, safuVecInsert(NULL, 2, &testInput));
}
