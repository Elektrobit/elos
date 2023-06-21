// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

int safuTestSafuJsonGetStringErrGetObjectFailedArraySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetStringErrGetObjectFailedArrayTeardown(UNUSED void **state) {
    return 0;
}

#define INITIAL_RESULT "initial state"
static void _testCase(size_t idx) {
    PARAM("idx = %lu", idx);
    const char *result = INITIAL_RESULT;

    int retval = safuJsonGetString(NULL, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_string_equal(result, INITIAL_RESULT);
}

void safuTestSafuJsonGetStringErrGetObjectFailedArray(UNUSED void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "fail to get a json object from a json array");

    for (size_t i = 0; i < 5; i++) {
        _testCase(i);
    }
}
