// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint64_utest.h"

int safuTestSafuJsonGetUint64ErrGetObjectFailedArraySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetUint64ErrGetObjectFailedArrayTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(size_t idx) {
    PARAM("idx = %lu", idx);
    uint64_t initResult = 117;
    uint64_t result = initResult;

    int retval = safuJsonGetUint64(NULL, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetUint64ErrGetObjectFailedArray(UNUSED void **state) {
    TEST("safuJsonGetUint64");
    SHOULD("%s", "fail to get a json object from a json array");

    for (size_t i = 0; i < 6; i++) {
        _testCase(i);
    }
}
