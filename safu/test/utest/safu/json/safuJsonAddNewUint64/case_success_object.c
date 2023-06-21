// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewUint64_utest.h"

SETUP(safuTestSafuJsonAddNewUint64SuccessObject)
TEARDOWN(safuTestSafuJsonAddNewUint64SuccessObject)

static void _testCase(struct json_object *jobj, const char *key, uint64_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %lu", val);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_uint64)(val);

    MOCK_FUNC_AFTER_CALL(json_object_new_uint64, 0);

    expect_value(__wrap_json_object_new_uint64, i, val);
    will_return(__wrap_json_object_new_uint64, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, key);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, json_object_object_add(jobj, key, jadd));

    struct json_object *juint64 = safuJsonAddNewUint64(jobj, key, val);
    assert_non_null(juint64);
    assert_ptr_equal(juint64, jadd);
    assert_int_equal(json_object_get_uint64(juint64), val);
    assert_int_equal(json_object_get_uint64(json_object_object_get(jobj, key)), val);
}

void safuTestSafuJsonAddNewUint64SuccessObject(void **state) {
    TEST("safuJsonAddNewUint64");
    SHOULD("%s", "successfully add a uint65_t to a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        uint64_t val;
        char *key;
    } testRows[] = {
        {0, "NULL"},
        {42, "fortytwo"},
        {INT64_MAX, "SomeThing"},
        {UINT64_MAX, "MAXIMUM"},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
