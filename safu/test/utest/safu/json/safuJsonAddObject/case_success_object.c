// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectObject)
TEARDOWN(safuTestSafuJsonAddObjectObject)

static void _testCase(struct json_object *jobj, const char *key, struct json_object *jdata) {
    PARAM("key = \"%s\"", key);

    MOCK_FUNC_AFTER_CALL(json_object_object_add, 0);

    expect_value(__wrap_json_object_object_add, obj, jobj);
    expect_value(__wrap_json_object_object_add, key, key);
    expect_value(__wrap_json_object_object_add, val, jdata);
    will_return(__wrap_json_object_object_add, __real_json_object_object_add(jobj, key, jdata));

    int retval = safuJsonAddObject(jobj, key, jdata);
    assert_int_equal(retval, 0);
    assert_ptr_equal(jdata, json_object_object_get(jobj, key));
}

void safuTestSafuJsonAddObjectObject(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "successfully add a json object to a json object");

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

    TEST("safuJsonAddObject");
    SHOULD("%s", "add a JSON object to a JSON object");

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].data);
    }
}
