// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewUint64_utest.h"

SETUP(safuTestSafuJsonAddNewUint64ErrAddObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewUint64ErrAddObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *name, uint64_t number) {
    PARAM("name = \"%s\"", name);
    PARAM("number = %lu", number);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_uint64)(number);

    MOCK_FUNC_AFTER_CALL(json_object_new_uint64, 0);

    expect_value(__wrap_json_object_new_uint64, i, number);
    will_return(__wrap_json_object_new_uint64, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, name);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *juint64 = safuJsonAddNewUint64(jobj, name, number);
    assert_null(juint64);
}

void safuTestSafuJsonAddNewUint64ErrAddObjectFailedObject(void **state) {
    TEST("safuJsonAddNewUint64");
    SHOULD("%s", "fail to add a new uint64_t to a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        uint64_t number;
        char *name;
    } testRows[] = {
        {0, "NULL"},
        {42, "fortytwo"},
        {INT64_MAX, "SomeThing"},
        {UINT64_MAX, "MAXIMUM"},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].number);
    }
}
