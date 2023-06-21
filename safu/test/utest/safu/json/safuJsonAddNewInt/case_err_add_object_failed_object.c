// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt_utest.h"

SETUP(safuTestSafuJsonAddNewIntErrAddObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewIntErrAddObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *key, int32_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %d", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_int)(val);

    MOCK_FUNC_AFTER_CALL(json_object_new_int, 0);

    expect_value(__wrap_json_object_new_int, i, val);
    will_return(__wrap_json_object_new_int, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, key);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jint = safuJsonAddNewInt(jobj, key, val);
    assert_null(jint);
}

void safuTestSafuJsonAddNewIntErrAddObjectFailedObject(void **state) {
    TEST("safuJsonAddNewInt");
    SHOULD("%s", "fail to add a new int to a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        int32_t val;
    } testRows[] = {
        {"NULL", 0}, {"fortytwo", 42}, {"MAXIMUM", INT32_MAX}, {"minus", -1}, {"MINIMUM", INT32_MIN},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
