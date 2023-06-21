// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewUint64_utest.h"

SETUP(safuTestSafuJsonAddNewUint64ErrAddObjectFailedArray)
TEARDOWN(safuTestSafuJsonAddNewUint64ErrAddObjectFailedArray)

static void _testCase(struct json_object *jarray, uint64_t val) {
    PARAM("val = %lu", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_uint64)(val);

    MOCK_FUNC_AFTER_CALL(json_object_new_uint64, 0);

    expect_value(__wrap_json_object_new_uint64, i, val);
    will_return(__wrap_json_object_new_uint64, jadd);

    expect_value(safuJsonAddObject, jobj, jarray);
    expect_value(safuJsonAddObject, name, NULL);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *juint64 = safuJsonAddNewUint64(jarray, NULL, val);
    assert_null(juint64);
}

void safuTestSafuJsonAddNewUint64ErrAddObjectFailedArray(void **state) {
    TEST("safuJsonAddNewUint64");
    SHOULD("%s", "fail to add a new json uint64 to a json array");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jarray = *(struct json_object **)state;
    assert_non_null(jarray);

    uint64_t testRows[] = {0, 42, INT64_MAX, UINT64_MAX};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jarray, testRows[i]);
    }
}
