// SPDX-License-Identifier: MIT
#include "cmocka_extensions/cmocka_extensions.h"
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectSuccessArray)
TEARDOWN(safuTestSafuJsonAddNewObjectSuccessArray)

void safuTestSafuJsonAddNewObjectSuccessArray(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "successfully add a new json object to a json array");

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
    will_return(safuJsonAddObject, json_object_array_add(jarray, jdata));

    struct json_object *result = safuJsonAddNewObject(jarray, NULL);
    assert_non_null(result);
    assert_ptr_equal(result, jdata);
    assert_ptr_equal(jdata, json_object_array_get_idx(jarray, 0));
}
