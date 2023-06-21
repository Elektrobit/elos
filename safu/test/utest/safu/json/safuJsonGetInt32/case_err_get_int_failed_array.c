// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetInt32_utest.h"

SETUP(safuTestSafuJsonGetInt32ErrGetIntArray)
TEARDOWN(safuTestSafuJsonGetInt32ErrGetIntArray)

static void _testCase(struct json_object *jarray, size_t idx, int32_t val) {
    PARAM("idx = %lu", idx);
    PARAM("val = %d", val);

    int32_t initResult = 117;
    int32_t result = initResult;

    MOCK_FUNC_AFTER_CALL(json_object_get_int, 0);

    expect_any(__wrap_json_object_get_int, obj);
    will_return_and_set_errno(__wrap_json_object_get_int, val, EINVAL);

    int retval = safuJsonGetInt32(jarray, NULL, idx, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetInt32ErrGetIntArray(void **state) {
    TEST("safuJsonGetInt32");
    SHOULD("%s", "return -1 when json_object_get_int fails and sets errno on a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    int32_t testRows[] = {0, 42, INT32_MAX, INT32_MIN, -12};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewInt(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i, testRows[i]);
    }
}
