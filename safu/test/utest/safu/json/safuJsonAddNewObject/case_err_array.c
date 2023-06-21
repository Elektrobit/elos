// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectErrArray)
TEARDOWN(safuTestSafuJsonAddNewObjectErrArray)

void safuTestSafuJsonAddNewObjectErrArray(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "fail to add a new json object to a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct json_object *jdata = MOCK_FUNC_REAL(json_object_new_object)();
    assert_non_null(jdata);

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    will_return(__wrap_json_object_new_object, jdata);

    expect_value(safuJsonAddObject, jobj, jarray);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jdata);
    will_return(safuJsonAddObject, -1);

    struct json_object *result = safuJsonAddNewObject(jarray, NULL);
    assert_null(result);
}
