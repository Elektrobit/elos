// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewUint64_utest.h"

SETUP(safuTestSafuJsonAddNewUint64ErrNewObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewUint64ErrNewObjectFailedArray)

static void _testCase(struct json_object *jarray, uint64_t value) {
    PARAM("value = %lu", value);

    MOCK_FUNC_AFTER_CALL(json_object_new_uint64, 0);

    expect_value(__wrap_json_object_new_uint64, i, value);
    will_return(__wrap_json_object_new_uint64, NULL);

    struct json_object *juint64 = safuJsonAddNewUint64(jarray, NULL, value);
    assert_null(juint64);
}

void safuTestSafuJsonAddNewUint64ErrNewObjectFailedArray(void **state) {
    TEST("safuJsonAddNewUint64");
    SHOULD("%s", "fail to chreate a new json uint64 to add to a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    uint64_t testRows[] = {0, 42, INT64_MAX, UINT64_MAX};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
