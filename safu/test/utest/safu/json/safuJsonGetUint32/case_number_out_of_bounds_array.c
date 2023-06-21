// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint32_utest.h"

SETUP(safuTestSafuJsonGetUint32NumberOutOfBoundsArray)
TEARDOWN(safuTestSafuJsonGetUint32NumberOutOfBoundsArray)

static void _testCase(struct json_object *jarray, size_t idx, uint64_t val) {
    PARAM("idx = %lu", idx);
    PARAM("val = %lu", val);
    uint32_t result;

    expect_value(safuJsonGetUint64, jobj, jarray);
    expect_value(safuJsonGetUint64, name, NULL);
    expect_value(safuJsonGetUint64, idx, idx);
    will_return(safuJsonGetUint64, val);
    will_return(safuJsonGetUint64, 0);

    int retval = safuJsonGetUint32(jarray, NULL, idx, &result);

    assert_int_equal(retval, 0);
    assert_int_equal(result, UINT32_MAX);
}

void safuTestSafuJsonGetUint32NumberOutOfBoundsArray(void **state) {
    TEST("safuJsonGetUint32");
    SHOULD("%s", "get a to large uint32 from a json array cliped down to UINT32_MAX");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;

    uint64_t testRows[] = {(uint64_t)UINT32_MAX + 1, UINT64_MAX};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewUint64(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i, testRows[i]);
    }
}
