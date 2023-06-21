// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectErrArray)
TEARDOWN(safuTestSafuJsonAddObjectErrArray)

static void _testCase(struct json_object *jarray, struct json_object *jdata) {
    MOCK_FUNC_AFTER_CALL(json_object_array_add, 0);

    expect_value(__wrap_json_object_array_add, obj, jarray);
    expect_value(__wrap_json_object_array_add, val, jdata);
    will_return(__wrap_json_object_array_add, -1);

    int retval = safuJsonAddObject(jarray, NULL, jdata);
    assert_int_equal(retval, -1);
    assert_int_equal(json_object_array_length(jarray), 0);
}

void safuTestSafuJsonAddObjectErrArray(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "fail to add a JSON object to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct json_object *testRows[] = {
        json_object_new_int(42),
        json_object_new_string("JSON String-Object"),
        json_object_new_object(),
        json_object_new_array(),
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        json_object_put(testRows[i]);
    }
}
