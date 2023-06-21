// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt32_utest.h"

int safuTestSafuJsonGetInt32ErrGetObjectFailedObjectSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetInt32ErrGetObjectFailedObjectTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(const char *key) {
    PARAM("key = \"%s\"", key);

    int32_t initResult = 117;
    int32_t result = initResult;

    int retval = safuJsonGetInt32(NULL, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetInt32ErrGetObjectFailedObject(UNUSED void **state) {
    TEST("safuJsonGetInt32");
    SHOULD("%s", "return -1 when safuJsonGetObject failes with an array index");

    char *testRows[] = {"Zero", "fortytwo", "Max", "Min", "minus"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(testRows[i]);
    }
}
