// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectErrNullKeyObject)
TEARDOWN(safuTestSafuJsonAddObjectErrNullKeyObject)

static void _testCase(struct json_object *jobj, struct json_object *jadd) {
    int retval = safuJsonAddObject(jobj, NULL, jadd);
    assert_int_equal(retval, -1);
    assert_int_equal(json_object_object_length(jobj), 0);
}

void safuTestSafuJsonAddObjectErrNullKeyObject(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "fail to add object with a NULL key to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct json_object *testRows[] = {
        json_object_new_int(42),
        json_object_new_string("JSON String-Object"),
        json_object_new_object(),
        json_object_new_array(),
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i]);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        json_object_put(testRows[i]);
    }
}
