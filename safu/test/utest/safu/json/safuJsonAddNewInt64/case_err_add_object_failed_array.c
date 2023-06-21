// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt64_utest.h"

SETUP(safuTestSafuJsonAddNewInt64ErrAddObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewInt64ErrAddObjectFailedArray)

static void _testCase(struct json_object *jarray, int64_t number) {
    PARAM("val = %ld", number);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_int64)(0);

    MOCK_FUNC_AFTER_CALL(json_object_new_int64, 0);

    expect_value(__wrap_json_object_new_int64, i, number);
    will_return(__wrap_json_object_new_int64, jadd);

    expect_value(safuJsonAddObject, jobj, jarray);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jint64 = safuJsonAddNewInt64(jarray, NULL, number);
    assert_null(jint64);
}

void safuTestSafuJsonAddNewInt64ErrAddObjectFailedArray(void **state) {
    TEST("safuJsonAddNewInt64");
    SHOULD("%s", "fail to add an int64_t to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    int64_t testRows[] = {0, 42, INT64_MAX, INT64_MIN};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
