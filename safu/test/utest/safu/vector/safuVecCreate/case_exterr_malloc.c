// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuVecCreate_utest.h"

void safuTestSafuVecCreateExterrMalloc(UNUSED void **state) {
    safuVec_t vec = {0};
    int32_t tdata[10] = {0};
    size_t preAllocatedElements = ARRAY_SIZE(tdata);

    TEST("safuVecCreate");
    SHOULD("%s", "return an error code due to failed malloc");

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, sizeof(tdata));
    will_return(__wrap_malloc, NULL);

    int retval = safuVecCreate(&vec, preAllocatedElements, sizeof(int32_t));

    assert_int_equal(retval, -1);
    assert_int_equal(vec.elementSize, 0);
    assert_int_equal(vec.memorySize, 0);
    assert_int_equal(vec.elementCount, 0);
    assert_ptr_equal(vec.data, NULL);
}
