// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt64_utest.h"

SETUP(safuTestSafuJsonAddNewInt64ErrNewObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewInt64ErrNewObjectFailedArray)

static void _testCase(struct json_object *jarray, int64_t number) {
    PARAM("val = %ld", number);

    MOCK_FUNC_AFTER_CALL(json_object_new_int64, 0);

    expect_value(__wrap_json_object_new_int64, i, 0);
    will_return(__wrap_json_object_new_int64, NULL);

    struct json_object *jint64 = safuJsonAddNewInt64(jarray, NULL, 0);
    assert_null(jint64);
}

void safuTestSafuJsonAddNewInt64ErrNewObjectFailedArray(void **state) {
    TEST("safuJsonAddNewInt64");
    SHOULD("%s", "fail to create an int64_t object to add to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    int64_t testRows[] = {0, 42, INT64_MAX, INT64_MIN};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
