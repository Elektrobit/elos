// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt_utest.h"

SETUP(safuTestSafuJsonAddNewIntErrAddObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewIntErrAddObjectFailedArray)

static void _testCase(struct json_object *jobj, int32_t val) {
    PARAM("val = %d", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_int)(val);

    MOCK_FUNC_AFTER_CALL(json_object_new_int, 0);

    expect_value(__wrap_json_object_new_int, i, val);
    will_return(__wrap_json_object_new_int, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jint = safuJsonAddNewInt(jobj, NULL, val);
    assert_null(jint);
}

void safuTestSafuJsonAddNewIntErrAddObjectFailedArray(void **state) {
    TEST("safuJsonAddNewInt");
    SHOULD("%s", "fail to add an object with an int to an array");
    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    int32_t testRows[] = {0, 42, INT32_MAX, -1, INT32_MIN};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
