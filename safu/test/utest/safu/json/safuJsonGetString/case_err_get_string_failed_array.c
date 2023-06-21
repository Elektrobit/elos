// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

SETUP(safuTestSafuJsonGetStringErrGetStringFailedArray)
TEARDOWN(safuTestSafuJsonGetStringErrGetStringFailedArray)

#define INITIAL_RESULT "initial state"
static void _testCase(const struct json_object *jobj, size_t idx) {
    PARAM("idx = %lu", idx);
    const char *result = INITIAL_RESULT;

    const char *retString = "some stuff";

    MOCK_FUNC_AFTER_CALL(json_object_get_string, 0);

    expect_any(__wrap_json_object_get_string, obj);
    will_return_and_set_errno(__wrap_json_object_get_string, retString, EINVAL);

    int retval = safuJsonGetString(jobj, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_string_equal(result, INITIAL_RESULT);
}

void safuTestSafuJsonGetStringErrGetStringFailedArray(void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "fail to get a string from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;

    char *testRows[] = {"one", "stuff", "", "somthing thats a bit longer and has spaces"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewString(jobj, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i);
    }
}
