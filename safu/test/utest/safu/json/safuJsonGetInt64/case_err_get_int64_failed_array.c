// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt64_utest.h"

SETUP(safuTestSafuJsonGetInt64ErrGetInt64FailedArray)
TEARDOWN(safuTestSafuJsonGetInt64ErrGetInt64FailedArray)

static void _testCase(struct json_object *jarray, size_t idx, int64_t val) {
    PARAM("idx = %lu", idx);
    PARAM("val = %ld", val);

    int64_t initResult = 117;
    int64_t result = initResult;

    MOCK_FUNC_AFTER_CALL(json_object_get_int64, 0);

    expect_any(__wrap_json_object_get_int64, obj);
    will_return_and_set_errno(__wrap_json_object_get_int64, val, EINVAL);

    int retval = safuJsonGetInt64(jarray, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetInt64ErrGetInt64FailedArray(void **state) {
    TEST("safuJsonGetInt64");
    SHOULD("%s", "return -1 when json_object_get_int fails and sets errno on a json array");

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
