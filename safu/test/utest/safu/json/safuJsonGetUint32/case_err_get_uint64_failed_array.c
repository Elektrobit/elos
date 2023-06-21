// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint32_utest.h"

SETUP(safuTestSafuJsonGetUint32ErrGetUint64FailedArray)
TEARDOWN(safuTestSafuJsonGetUint32ErrGetUint64FailedArray)

static void _testCase(struct json_object *jarray, size_t idx) {
    PARAM("idx = %lu", idx);
    uint32_t initResult = 117;
    uint32_t result = initResult;

    expect_value(safuJsonGetUint64, jobj, jarray);
    expect_value(safuJsonGetUint64, name, NULL);
    expect_value(safuJsonGetUint64, idx, idx);
    will_return(safuJsonGetUint64, 0);
    will_return(safuJsonGetUint64, -1);

    int retval = safuJsonGetUint32(jarray, NULL, idx, &result);

    assert_int_not_equal(retval, 0);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetUint32ErrGetUint64FailedArray(void **state) {
    TEST("safuJsonGetUint32");
    SHOULD("%s", "fail to get an uint32 from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;

    uint64_t testRows[] = {0, 42, UINT32_MAX};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewUint64(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i);
    }
}
