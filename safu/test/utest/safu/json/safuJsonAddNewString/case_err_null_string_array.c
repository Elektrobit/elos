// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrNullStringArray)
TEARDOWN(safuTestSafuJsonAddNewStringErrNullStringArray)

void safuTestSafuJsonAddNewStringErrNullStringArray(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail add NULL instead of a string to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct json_object *jstr = safuJsonAddNewString(jarray, NULL, NULL);
    assert_null(jstr);
    assert_int_equal(json_object_array_length(jarray), 0);
}
