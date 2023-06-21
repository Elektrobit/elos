// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt_utest.h"

SETUP(safuTestSafuJsonAddNewIntSuccessArray)
TEARDOWN(safuTestSafuJsonAddNewIntSuccessArray)

static void _testCase(struct json_object *jobj, size_t idx, int32_t val) {
    PARAM("idx = %ld", idx);
    PARAM("val = %d", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_int)(val);
    assert_non_null(jadd);

    MOCK_FUNC_AFTER_CALL(json_object_new_int, 0);

    expect_value(__wrap_json_object_new_int, i, val);
    will_return(__wrap_json_object_new_int, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, json_object_array_add(jobj, jadd));

    struct json_object *jint = safuJsonAddNewInt(jobj, NULL, val);
    assert_non_null(jint);
    assert_ptr_equal(jint, jadd);
    assert_int_equal(json_object_get_int(jint), val);
    assert_int_equal(json_object_get_int(json_object_array_get_idx(jobj, idx)), val);
}

void safuTestSafuJsonAddNewIntSuccessArray(void **state) {
    TEST("safuJsonAddNewInt");
    SHOULD("%s", "succesfully add a new int to a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    int32_t testRows[] = {0, 42, INT32_MAX, -1, INT32_MIN};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i, testRows[i]);
    }
}
