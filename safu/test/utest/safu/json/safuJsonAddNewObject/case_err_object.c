// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectErrObject)
TEARDOWN(safuTestSafuJsonAddNewObjectErrObject)

void safuTestSafuJsonAddNewObjectErrObject(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "fail to add a new json object to a json object");

    *(struct json_object **)state = MOCK_FUNC_REAL(json_object_new_object)();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    char *key = "first";
    struct json_object *jdata = MOCK_FUNC_REAL(json_object_new_object)();
    assert_non_null(jdata);

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    will_return(__wrap_json_object_new_object, jdata);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, key);
    expect_value(safuJsonAddObject, jdata, jdata);
    will_return(safuJsonAddObject, -1);

    struct json_object *result = safuJsonAddNewObject(jobj, key);
    assert_null(result);
}
