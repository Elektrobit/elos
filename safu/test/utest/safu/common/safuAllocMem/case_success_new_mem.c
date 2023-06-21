// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuAllocMem_utest.h"

void safuTestSafuAllocMemSuccessNewMem(UNUSED void **state) {
    void *oldptr = (void *)0xdeadbeef, *newptr;
    const size_t newlen = 123;

    TEST("safuAllocMem");
    SHOULD("%s", "return pointer to new memory for valid parameters");

    MOCK_FUNC_AFTER_CALL(realloc, 0);

    expect_value(__wrap_realloc, ptr, oldptr);
    expect_value(__wrap_realloc, newSize, newlen);
    will_return(__wrap_realloc, (void *)0xc0ffee);

    newptr = safuAllocMem(oldptr, newlen);

    assert_ptr_equal(newptr, (void *)0xc0ffee);
}
