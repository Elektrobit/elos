// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuAllocMem_utest.h"

void safuTestSafuAllocMemSuccessNewlenZero(UNUSED void **state) {
    void *oldptr = (void *)0xdeadbeef, *newptr;
    const size_t newlen = 0;

    TEST("safuAllocMem");
    SHOULD("%s", "return NULL for newlen being zero");

    MOCK_FUNC_AFTER_CALL(free, 0);

    expect_value(__wrap_free, ptr, oldptr);

    newptr = safuAllocMem(oldptr, newlen);

    assert_null(newptr);
}
