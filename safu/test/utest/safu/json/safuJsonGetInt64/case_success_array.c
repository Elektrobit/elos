// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt64_utest.h"

SETUP(safuTestSafuJsonGetInt64SuccessArray)
TEARDOWN(safuTestSafuJsonGetInt64SuccessArray)

static void _testCase(struct json_object *jarray, size_t idx, int64_t val) {
    PARAM("idx = %lu", idx);
    PARAM("val = %ld", val);

    int64_t result;

    MOCK_FUNC_AFTER_CALL(json_object_get_int64, 0);

    expect_any(__wrap_json_object_get_int64, obj);
    will_return(__wrap_json_object_get_int64, val);

    int retval = safuJsonGetInt64(jarray, NULL, idx, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result, val);
}

void safuTestSafuJsonGetInt64SuccessArray(void **state) {
    TEST("safuJsonGetInt64");
    SHOULD("%s", "successfully get an int64 from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    int64_t testRows[] = {0, 42, INT64_MAX, INT64_MIN, -12};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewInt64(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i, testRows[i]);
    }
}
