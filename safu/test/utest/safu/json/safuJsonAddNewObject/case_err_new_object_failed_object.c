// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewObjectErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *key) {
    PARAM("key = %s", key);

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    will_return(__wrap_json_object_new_object, NULL);

    struct json_object *result = safuJsonAddNewObject(jobj, key);
    assert_null(result);
}

void safuTestSafuJsonAddNewObjectErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "fail to create a new object to add to a json object");

    *(struct json_object **)state = MOCK_FUNC_REAL(json_object_new_object)();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    char *testRows[] = {"first", "Next"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
