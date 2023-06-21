// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrAddObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewStringErrAddObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *name, const char *val) {
    PARAM("val = %s", name);
    PARAM("val = %s", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_string)(val);
    assert_non_null(jadd);

    MOCK_FUNC_AFTER_CALL(json_object_new_string, 0);

    expect_value(__wrap_json_object_new_string, s, val);
    will_return(__wrap_json_object_new_string, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, name);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jstr = safuJsonAddNewString(jobj, name, val);
    assert_null(jstr);
    assert_int_equal(json_object_object_length(jobj), 0);
}

void safuTestSafuJsonAddNewStringErrAddObjectFailedObject(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail to add a string to a JSON object");

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
