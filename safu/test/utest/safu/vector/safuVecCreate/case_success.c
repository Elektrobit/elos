// SPDX-License-Identifier: MIT
#include <stdint.h>

#include "cmocka_mocks/mock_libc.h"
#include "safuVecCreate_utest.h"

void safuTestSafuVecCreateSuccess(UNUSED void **state) {
    safuVec_t vec = {0};
    int32_t tdata[10] = {0};
    size_t preAllocatedElements = ARRAY_SIZE(tdata);
    int retVal;

    TEST("safuVecCreate");
    SHOULD("%s", "properly allocate and initialize the vec_t");

    PARAM("Initialize vector with elementCount == 0");

    retVal = safuVecCreate(&vec, 0, sizeof(int32_t));
    assert_int_equal(retVal, 0);
    assert_int_equal(vec.elementSize, sizeof(int32_t));
    assert_int_equal(vec.memorySize, 0);
    assert_int_equal(vec.elementCount, 0);
    assert_ptr_equal(vec.data, NULL);

    memset(&vec, 0, sizeof(safuVec_t));

    PARAM("Initialize vector with elementCount > 0");

    MOCK_FUNC_AFTER_CALL(malloc, 0);
    expect_value(__wrap_malloc, size, sizeof(tdata));
    will_return(__wrap_malloc, &tdata);

    retVal = safuVecCreate(&vec, preAllocatedElements, sizeof(int32_t));
    assert_int_equal(retVal, 0);
    assert_int_equal(vec.elementSize, sizeof(int32_t));
    assert_int_equal(vec.memorySize, sizeof(tdata));
    assert_int_equal(vec.elementCount, 0);
    assert_ptr_equal(vec.data, &tdata);
}
