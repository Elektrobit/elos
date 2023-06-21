// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetString_utest.h"

SETUP(safuTestSafuJsonGetStringSuccessArray)
TEARDOWN(safuTestSafuJsonGetStringSuccessArray)

static void _testCase(const struct json_object *jobj, size_t idx, const char *val) {
    PARAM("idx = %lu", idx);
    PARAM("val = \"%s\"", val);
    const char *result;

    MOCK_FUNC_AFTER_CALL(json_object_get_string, 0);

    expect_any(__wrap_json_object_get_string, obj);
    will_return(__wrap_json_object_get_string, val);

    int retval = safuJsonGetString(jobj, NULL, idx, &result);

    assert_int_equal(retval, 0);
    assert_string_equal(result, val);
}

void safuTestSafuJsonGetStringSuccessArray(void **state) {
    TEST("safuJsonGetString");
    SHOULD("%s", "successfully get a string from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;

    char *testRows[] = {"one", "stuff", "", "somthing thats a bit longer and has spaces"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewString(jobj, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i, testRows[i]);
    }
}
