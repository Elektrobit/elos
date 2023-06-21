// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuAllocMem_utest.h"

void safuTestSafuAllocMemSuccessAllParamZero(UNUSED void **state) {
    void *oldptr = NULL, *newptr;
    const size_t newlen = 0;

    TEST("safuAllocMem");
    SHOULD("%s", "return NULL when all parameters are NULL");

    MOCK_FUNC_AFTER_CALL(realloc, 0);

    expect_value(__wrap_realloc, ptr, oldptr);
    expect_value(__wrap_realloc, newSize, newlen);
    will_return(__wrap_realloc, NULL);

    newptr = safuAllocMem(oldptr, newlen);

    assert_null(newptr);
}
