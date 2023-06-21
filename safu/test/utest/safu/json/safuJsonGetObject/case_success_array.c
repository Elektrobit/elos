// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetObject_utest.h"

SETUP(safuTestSafuJsonGetObjectSuccessArray)
TEARDOWN(safuTestSafuJsonGetObjectSuccessArray)

static void _testCase(struct json_object *jarray, size_t idx, struct json_object *data) {
    PARAM("idx = %lu", idx);

    MOCK_FUNC_AFTER_CALL(json_object_array_get_idx, 0);

    expect_value(__wrap_json_object_array_get_idx, obj, jarray);
    expect_value(__wrap_json_object_array_get_idx, idx, idx);
    will_return(__wrap_json_object_array_get_idx, data);

    struct json_object *res = safuJsonGetObject(jarray, NULL, idx);

    assert_ptr_equal(res, data);
}

void safuTestSafuJsonGetObjectSuccessArray(void **state) {
    TEST("safuJsonGetObject");
    SHOULD("%s", "successfully get a json object from a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct json_object *testRows[] = {
        json_object_new_int(42),  json_object_new_string("JSON String-Object"),
        json_object_new_object(), json_object_new_array(),
        json_object_new_null(),
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddObject(jarray, NULL, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, i, testRows[i]);
    }
}
