// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewStringErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, char *name, char *val) {
    PARAM("val = %s", name);
    PARAM("val = %s", val);

    MOCK_FUNC_AFTER_CALL(json_object_new_string, 0);

    expect_value(__wrap_json_object_new_string, s, val);
    will_return(__wrap_json_object_new_string, NULL);

    struct json_object *jstr = safuJsonAddNewString(jobj, name, val);
    assert_null(jstr);
    assert_int_equal(json_object_object_length(jobj), 0);
}

void safuTestSafuJsonAddNewStringErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail to create a new string to add to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *name;
        char *val;
    } testRows[] = {
        {"first", "one"},
        {"", "stuff"},
        {"empty", ""},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].val);
    }
}
