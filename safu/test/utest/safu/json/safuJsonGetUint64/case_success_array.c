// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint64_utest.h"

SETUP(safuTestSafuJsonGetUint64SuccessArray)
TEARDOWN(safuTestSafuJsonGetUint64SuccessArray)

static void _testCase(const struct json_object *jarray, size_t idx, uint64_t val) {
    PARAM("idx = %lu", idx);
    PARAM("val = %lu", val);
    uint64_t result;

    MOCK_FUNC_AFTER_CALL(json_object_get_uint64, 0);

    expect_any(__wrap_json_object_get_uint64, obj);
    will_return(__wrap_json_object_get_uint64, val);

    int retval = safuJsonGetUint64(jarray, NULL, idx, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result, val);
}

void safuTestSafuJsonGetUint64SuccessArray(void **state) {
    TEST("safuJsonGetUint64");
    SHOULD("%s", "successfully get an uint64 from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    uint64_t testRows[] = {0, 42, UINT64_MAX};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewUint64(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i, testRows[i]);
    }
}
