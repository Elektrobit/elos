// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint64_utest.h"

int safuTestSafuJsonGetUint64ErrGetObjectFailedObjectSetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuJsonGetUint64ErrGetObjectFailedObjectTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(char *key) {
    PARAM("key = \"%s\"", key);
    uint64_t initResult = 117;
    uint64_t result = initResult;

    int retval = safuJsonGetUint64(NULL, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetUint64ErrGetObjectFailedObject(UNUSED void **state) {
    TEST("safuJsonGetUint64");
    SHOULD("%s", "fail to get a json object from a json object");

    char *testRows[] = {"Zero", "fortytwo", "Max", "Min", "minus"};
    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(testRows[i]);
    }
}
