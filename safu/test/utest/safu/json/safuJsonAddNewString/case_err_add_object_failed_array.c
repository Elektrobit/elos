// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrAddObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewStringErrAddObjectFailedArray)

static void _testCase(struct json_object *jarray, const char *val) {
    PARAM("val = %s", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_string)(val);
    assert_non_null(jadd);

    MOCK_FUNC_AFTER_CALL(json_object_new_string, 0);

    expect_value(__wrap_json_object_new_string, s, val);
    will_return(__wrap_json_object_new_string, jadd);

    expect_value(safuJsonAddObject, jobj, jarray);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jstr = safuJsonAddNewString(jarray, NULL, val);
    assert_null(jstr);
    assert_int_equal(json_object_array_length(jarray), 0);
}

void safuTestSafuJsonAddNewStringErrAddObjectFailedArray(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail to add a string to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    char *testRows[] = {"one", "stuff", ""};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
