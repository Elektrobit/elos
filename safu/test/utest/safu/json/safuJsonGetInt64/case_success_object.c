// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt64_utest.h"

SETUP(safuTestSafuJsonGetInt64SuccessObject)
TEARDOWN(safuTestSafuJsonGetInt64SuccessObject)

static void _testCase(struct json_object *jobj, const char *key, int64_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %ld", val);

    int64_t result;

    MOCK_FUNC_AFTER_CALL(json_object_get_int64, 0);

    expect_any(__wrap_json_object_get_int64, obj);
    will_return(__wrap_json_object_get_int64, val);

    int retval = safuJsonGetInt64(jobj, key, 0, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result, val);
}

void safuTestSafuJsonGetInt64SuccessObject(void **state) {
    TEST("safuJsonGetInt64");
    SHOULD("%s", "successfully get an int64 from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        int64_t val;
    } testRows[] = {{"Zero", 0}, {"fortytwo", 42}, {"Max", INT64_MAX}, {"Min", INT64_MIN}, {"minus", -12}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewInt64(jobj, testRows[i].key, testRows[i].val);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
