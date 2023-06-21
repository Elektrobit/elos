// SPDX-License-Identifier: MIT
#include <errno.h>

#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt32_utest.h"

int safuTestSafuJsonGetInt32ErrGetObjectFailedArraySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetInt32ErrGetObjectFailedArrayTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(size_t idx) {
    PARAM("idx = %lu", idx);

    int32_t initResult = 117;
    int32_t result = initResult;

    int retval = safuJsonGetInt32(NULL, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetInt32ErrGetObjectFailedArray(UNUSED void **state) {
    TEST("safuJsonGetInt32");
    SHOULD("%s", "return -1 when safuJsonGetObject failes with an array index");

    for (size_t i = 0; i < 6; i++) {
        _testCase(i);
    }
}
