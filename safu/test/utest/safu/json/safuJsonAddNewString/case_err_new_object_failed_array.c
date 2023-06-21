// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrNewObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewStringErrNewObjectFailedArray)

static void _testCase(struct json_object *jarray, char *val) {
    PARAM("val = %s", val);

    MOCK_FUNC_AFTER_CALL(json_object_new_string, 0);

    expect_value(__wrap_json_object_new_string, s, val);
    will_return(__wrap_json_object_new_string, NULL);

    struct json_object *jstr = safuJsonAddNewString(jarray, NULL, val);
    assert_null(jstr);
    assert_int_equal(json_object_array_length(jarray), 0);
}

void safuTestSafuJsonAddNewStringErrNewObjectFailedArray(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail to create a new string object to add to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;

    char *testRows[] = {"one", "stuff", ""};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
