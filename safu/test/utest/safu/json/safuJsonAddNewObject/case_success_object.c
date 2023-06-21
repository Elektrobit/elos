// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectSuccessObject)
TEARDOWN(safuTestSafuJsonAddNewObjectSuccessObject)

static void _testCase(struct json_object *jobj, const char *key) {
    PARAM("key = %s", key);

    struct json_object *jdata = MOCK_FUNC_REAL(json_object_new_object)();
    assert_non_null(jdata);

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    will_return(__wrap_json_object_new_object, jdata);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, key);
    expect_value(safuJsonAddObject, jdata, jdata);
    will_return(safuJsonAddObject, json_object_object_add(jobj, key, jdata));

    struct json_object *result = safuJsonAddNewObject(jobj, key);
    assert_non_null(result);
    assert_ptr_equal(jdata, result);
    assert_ptr_equal(jdata, json_object_object_get(jobj, key));
}

void safuTestSafuJsonAddNewObjectSuccessObject(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "successfully add a new json object to a json object");

    *(struct json_object **)state = MOCK_FUNC_REAL(json_object_new_object)();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    char *testRows[] = {"first", "Next"};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }
}
