// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewString_utest.h"

SETUP(safuTestSafuJsonAddNewStringErrNullStringObject)
TEARDOWN(safuTestSafuJsonAddNewStringErrNullStringObject)

void safuTestSafuJsonAddNewStringErrNullStringObject(void **state) {
    TEST("safuJsonAddNewString");
    SHOULD("%s", "fail to add NULL insted of a string to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct json_object *jstr = safuJsonAddNewString(jobj, "Null", NULL);
    assert_null(jstr);
    assert_int_equal(json_object_object_length(jobj), 0);
}
