// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewObject_utest.h"

SETUP(safuTestSafuJsonAddNewObjectErrNewObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewObjectErrNewObjectFailedArray)

void safuTestSafuJsonAddNewObjectErrNewObjectFailedArray(void **state) {
    TEST("safuJsonAddNewObject");
    SHOULD("%s", "fail to create a new json objct to add to a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    MOCK_FUNC_AFTER_CALL(json_object_new_object, 0);

    will_return(__wrap_json_object_new_object, NULL);

    struct json_object *result = safuJsonAddNewObject(jarray, NULL);
    assert_null(result);
}
