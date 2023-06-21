// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_libc.h"
#include "safuVecInsert_utest.h"

void safuTestSafuVecInsertErrInvalidIndex(UNUSED void **state) {
    safuTdata_t testInput = {420, "420"};
    const size_t initialSize = 10;
    safuVec_t vec = {0};

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, initialSize * sizeof(safuTdata_t));
    will_return(__wrap_malloc, __real_malloc(initialSize * sizeof(safuTdata_t)));

    int retval = safuVecCreate(&vec, initialSize, sizeof(safuTdata_t));
    assert_int_equal(retval, 0);

    for (size_t i = 0; i < initialSize; i++) {
        retval = safuVecInsert(&vec, vec.elementCount + 1, &testInput);
        assert_int_equal(retval, -1);
        retval = safuVecInsert(&vec, initialSize + 1, &testInput);
        assert_int_equal(retval, -1);

        retval = safuVecInsert(&vec, vec.elementCount, &testInput);
        assert_int_equal(retval, 0);
    }

    for (size_t i = initialSize; i < initialSize + 3; i++) {
        retval = safuVecInsert(&vec, vec.elementCount + 1, &testInput);
        assert_int_equal(retval, -1);
        retval = safuVecInsert(&vec, vec.memorySize / vec.elementSize + 1, &testInput);
        assert_int_equal(retval, -1);

        MOCK_FUNC_AFTER_CALL(realloc, 0);
        expect_value(__wrap_realloc, ptr, vec.data);
        expect_value(__wrap_realloc, newSize, (i + 1) * sizeof(safuTdata_t));
        will_return(__wrap_realloc, __real_realloc(vec.data, (i + 1) * sizeof(safuTdata_t)));

        retval = safuVecInsert(&vec, vec.elementCount, &testInput);
        assert_int_equal(retval, 0);
    }

    retval = safuVecInsert(&vec, UINT32_MAX, &testInput);
    assert_int_equal(retval, -1);

    safuVecFree(&vec);
}
