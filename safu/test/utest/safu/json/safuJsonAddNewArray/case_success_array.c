// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewArray_utest.h"

SETUP(safuTestSafuJsonAddNewArraySuccessArray)
TEARDOWN(safuTestSafuJsonAddNewArraySuccessArray)

static void _testCase(struct json_object *jarray, size_t idx) {
    PARAM("idx = %ld", idx);
    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_array)();
    assert_non_null(jadd);

    MOCK_FUNC_AFTER_CALL(json_object_new_array, 0);

    will_return(__wrap_json_object_new_array, jadd);

    expect_value(safuJsonAddObject, jobj, jarray);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, json_object_array_add(jarray, jadd));

    struct json_object *jarr = safuJsonAddNewArray(jarray, NULL);
    assert_non_null(jarr);
    assert_ptr_equal(jarr, jadd);
    assert_ptr_equal(json_object_array_get_idx(jarray, idx), jadd);
}

void safuTestSafuJsonAddNewArraySuccessArray(void **state) {
    TEST("safuJsonAddNewArray");
    SHOULD("%s", "add an empty array to a JSON array");

    *(struct json_object **)state = MOCK_FUNC_REAL(json_object_new_array)();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    for (size_t i = 0; i < 4; i++) {
        _testCase(jarray, i);
    }
}
