// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewArray_utest.h"

SETUP(safuTestSafuJsonAddNewArrayErrNewObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewArrayErrNewObjectFailedArray)

static void _testCase(struct json_object *jarray) {
    MOCK_FUNC_AFTER_CALL(json_object_new_array, 0);

    will_return(__wrap_json_object_new_array, NULL);

    struct json_object *jarr = safuJsonAddNewArray(jarray, NULL);
    assert_null(jarr);
}

void safuTestSafuJsonAddNewArrayErrNewObjectFailedArray(void **state) {
    TEST("safuJsonAddNewArray");
    SHOULD("%s", "fail to create a new array to add to a JSON array");

    *(struct json_object **)state = MOCK_FUNC_REAL(json_object_new_array)();
    struct json_object *jarray = *(struct json_object **)state;

    assert_non_null(jarray);

    for (size_t i = 0; i < 4; i++) {
        _testCase(jarray);
    }
}
