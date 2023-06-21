// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewArray_utest.h"

SETUP(safuTestSafuJsonAddNewArrayErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewArrayErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, char *name) {
    PARAM("name = %s", name);

    MOCK_FUNC_AFTER_CALL(json_object_new_array, 0);

    will_return(__wrap_json_object_new_array, NULL);

    struct json_object *jarr = safuJsonAddNewArray(jobj, name);
    assert_null(jarr);
}

void safuTestSafuJsonAddNewArrayErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewArray");
    SHOULD("%s", "fail to create an empty array to add to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    assert_non_null(jobj);

    char *testRows[] = {"one", "anotherone", "and again"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
