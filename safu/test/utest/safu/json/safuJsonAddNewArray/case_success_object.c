// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewArray_utest.h"

SETUP(safuTestSafuJsonAddNewArraySuccessObject)
TEARDOWN(safuTestSafuJsonAddNewArraySuccessObject)

static void _testCase(struct json_object *jobj, char *name) {
    PARAM("name = %s", name);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_array)();
    assert_non_null(jadd);

    MOCK_FUNC_AFTER_CALL(json_object_new_array, 0);

    will_return(__wrap_json_object_new_array, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, name);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, json_object_object_add(jobj, name, jadd));

    struct json_object *jarr = safuJsonAddNewArray(jobj, name);
    assert_non_null(jarr);
    assert_ptr_equal(jarr, jadd);
    assert_ptr_equal(json_object_object_get(jobj, name), jadd);
}

void safuTestSafuJsonAddNewArraySuccessObject(void **state) {
    TEST("safuJsonAddNewArray");
    SHOULD("%s", "add an empty array to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    assert_non_null(jobj);

    char *testRows[] = {"one", "anotherone", "and again"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
