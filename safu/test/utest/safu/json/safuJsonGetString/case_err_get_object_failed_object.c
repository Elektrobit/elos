// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

int safuTestSafuJsonGetStringErrGetObjectFailedObjectSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetStringErrGetObjectFailedObjectTeardown(UNUSED void **state) {
    return 0;
}

#define INITIAL_RESULT "initial state"
static void _testCase(const char *key) {
    PARAM("key = \"%s\"", key);
    const char *result = INITIAL_RESULT;

    int retval = safuJsonGetString(NULL, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_string_equal(result, INITIAL_RESULT);
}

void safuTestSafuJsonGetStringErrGetObjectFailedObject(UNUSED void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "fail to get a json object from a json object");

    char *testRows[] = {
        "first",
        "",
        "empty",
        "longer string",
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(testRows[i]);
    }
}
