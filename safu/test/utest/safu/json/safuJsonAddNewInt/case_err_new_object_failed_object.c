// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt_utest.h"

SETUP(safuTestSafuJsonAddNewIntErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewIntErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *key, int32_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %d", val);

    MOCK_FUNC_AFTER_CALL(json_object_new_int, 0);

    expect_value(__wrap_json_object_new_int, i, val);
    will_return(__wrap_json_object_new_int, NULL);

    struct json_object *jint = safuJsonAddNewInt(jobj, key, val);
    assert_null(jint);
}

void safuTestSafuJsonAddNewIntErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewInt");
    SHOULD("%s", "fail to create a json int object to add to a json objct");

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
