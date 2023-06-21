// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt64_utest.h"

int safuTestSafuJsonGetInt64ErrGetObjectFailedArraySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetInt64ErrGetObjectFailedArrayTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(size_t idx) {
    PARAM("idx = %lu", idx);

    int64_t initResult = 117;
    int64_t result = initResult;

    int retval = safuJsonGetInt64(NULL, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetInt64ErrGetObjectFailedArray(UNUSED void **state) {
    TEST("safuJsonGetInt64");
    SHOULD("%s", "return -1 when safuJsonGetObject failes with an array index");

    for (size_t i = 0; i < 6; i++) {
        _testCase(i);
    }
}
