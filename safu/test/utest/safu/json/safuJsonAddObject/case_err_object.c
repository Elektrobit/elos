// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectErrObject)
TEARDOWN(safuTestSafuJsonAddObjectErrObject)

static void _testCase(struct json_object *jobj, const char *key, struct json_object *jdata) {
    PARAM("key = \"%s\"", key);

    MOCK_FUNC_AFTER_CALL(json_object_object_add, 0);

    expect_value(__wrap_json_object_object_add, obj, jobj);
    expect_value(__wrap_json_object_object_add, key, key);
    expect_value(__wrap_json_object_object_add, val, jdata);
    will_return(__wrap_json_object_object_add, -1);

    int retval = safuJsonAddObject(jobj, key, jdata);
    assert_int_equal(retval, -1);
    assert_int_equal(json_object_object_length(jobj), 0);
}

void safuTestSafuJsonAddObjectErrObject(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "fail to add a JSON object to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *name;
        struct json_object *data;
    } testRows[] = {
        {"fortytwo", json_object_new_int(42)}, {"string", json_object_new_string("JSON String-Object")},
        {"object", json_object_new_object()},  {"array", json_object_new_array()},
        {"", json_object_new_null()},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].data);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        json_object_put(testRows[i].data);
    }
}
