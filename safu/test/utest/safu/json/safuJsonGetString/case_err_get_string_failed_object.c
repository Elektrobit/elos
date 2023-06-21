// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

SETUP(safuTestSafuJsonGetStringErrGetStringFailedObject)
TEARDOWN(safuTestSafuJsonGetStringErrGetStringFailedObject)

#define INITIAL_RESULT "initial state"
static void _testCase(const struct json_object *jobj, const char *key) {
    PARAM("key = \"%s\"", key);
    const char *result = INITIAL_RESULT;

    const char *retString = "some stuff";

    MOCK_FUNC_AFTER_CALL(json_object_get_string, 0);

    expect_any(__wrap_json_object_get_string, obj);
    will_return_and_set_errno(__wrap_json_object_get_string, retString, EINVAL);

    int retval = safuJsonGetString(jobj, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_string_equal(result, INITIAL_RESULT);
}

void safuTestSafuJsonGetStringErrGetStringFailedObject(void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "fail to get a string from a json object");

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
        _testCase(jobj, testRows[i].name);
    }
}
