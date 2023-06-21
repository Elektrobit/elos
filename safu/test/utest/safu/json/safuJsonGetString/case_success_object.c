// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

SETUP(safuTestSafuJsonGetStringSuccessObject)
TEARDOWN(safuTestSafuJsonGetStringSuccessObject)

static void _testCase(const struct json_object *jobj, const char *key, const char *val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = \"%s\"", val);
    const char *result;

    MOCK_FUNC_AFTER_CALL(json_object_get_string, 0);

    expect_any(__wrap_json_object_get_string, obj);
    will_return(__wrap_json_object_get_string, val);

    int retval = safuJsonGetString(jobj, key, 0, &result);

    assert_int_equal(retval, 0);
    assert_string_equal(result, val);
}

void safuTestSafuJsonGetStringSuccessObject(void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "successfully get a string from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    struct {
        char *name;
        char *val;
    } testRows[] = {
        {"first", "one"},
        {"", "stuff"},
        {"empty", ""},
        {"longer string", "somthing thats a bit longer and has spaces"},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewString(jobj, testRows[i].name, testRows[i].val);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].val);
    }
}
