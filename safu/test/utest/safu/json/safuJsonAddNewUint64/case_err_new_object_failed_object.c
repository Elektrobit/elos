// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewUint64_utest.h"

SETUP(safuTestSafuJsonAddNewUint64ErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewUint64ErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *key, uint64_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %lu", val);

    MOCK_FUNC_AFTER_CALL(json_object_new_uint64, 0);

    expect_value(__wrap_json_object_new_uint64, i, 0);
    will_return(__wrap_json_object_new_uint64, NULL);

    struct json_object *juint64 = safuJsonAddNewUint64(jobj, "NULL", 0);
    assert_null(juint64);
}

void safuTestSafuJsonAddNewUint64ErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewUint64");
    SHOULD("%s", "fail to create a new json uint64 to add to a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        uint64_t val;
    } testRows[] = {
        {"NULL", 0},
        {"fortytwo", 42},
        {"SomeThing", INT64_MAX},
        {"MAXIMUM", UINT64_MAX},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
