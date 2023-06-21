// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt32_utest.h"

SETUP(safuTestSafuJsonGetInt32SuccessObject)
TEARDOWN(safuTestSafuJsonGetInt32SuccessObject)

static void _testCase(struct json_object *jobj, const char *key, int32_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %d", val);

    int32_t result;

    MOCK_FUNC_AFTER_CALL(json_object_get_int, 0);

    expect_any(__wrap_json_object_get_int, obj);
    will_return(__wrap_json_object_get_int, val);

    int retval = safuJsonGetInt32(jobj, key, 0, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result, val);
}

void safuTestSafuJsonGetInt32SuccessObject(void **state) {
    TEST("safuJsonGetInt32");
    SHOULD("%s", "successfully get an int32 from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        int32_t val;
    } testRows[] = {{"Zero", 0}, {"fortytwo", 42}, {"Max", INT32_MAX}, {"Min", INT32_MIN}, {"minus", -12}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewInt(jobj, testRows[i].key, testRows[i].val);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
