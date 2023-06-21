// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddObject_utest.h"

SETUP(safuTestSafuJsonAddObjectArray)
TEARDOWN(safuTestSafuJsonAddObjectArray)

static void _testCase(struct json_object *jarray, struct json_object *jdata, size_t idx) {
    PARAM("idx = %lu", idx);

    MOCK_FUNC_AFTER_CALL(json_object_array_add, 0);

    expect_value(__wrap_json_object_array_add, obj, jarray);
    expect_value(__wrap_json_object_array_add, val, jdata);
    will_return(__wrap_json_object_array_add, __real_json_object_array_add(jarray, jdata));

    int retval = safuJsonAddObject(jarray, NULL, jdata);
    assert_int_equal(retval, 0);
    assert_ptr_equal(jdata, json_object_array_get_idx(jarray, idx));
}

void safuTestSafuJsonAddObjectArray(void **state) {
    TEST("safuJsonAddObject");
    SHOULD("%s", "add a JSON object to a JSON array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    struct json_object *testRows[] = {
        json_object_new_int(42),
        json_object_new_string("JSON String-Object"),
        json_object_new_object(),
        json_object_new_array(),
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i], i);
    }
}
