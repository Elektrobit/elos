// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt64_utest.h"

SETUP(safuTestSafuJsonAddNewInt64ErrNewObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewInt64ErrNewObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *name, int64_t number) {
    PARAM("name = %s", name);
    PARAM("val = %ld", number);

    MOCK_FUNC_AFTER_CALL(json_object_new_int64, 0);

    expect_value(__wrap_json_object_new_int64, i, number);
    will_return(__wrap_json_object_new_int64, NULL);

    struct json_object *jint64 = safuJsonAddNewInt64(jobj, name, number);
    assert_null(jint64);
}

void safuTestSafuJsonAddNewInt64ErrNewObjectFailedObject(void **state) {
    TEST("safuJsonAddNewInt64");
    SHOULD("%s", "fail to create an int64 object to add to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *name;
        int64_t number;
    } testRows[] = {{"NULL", 0}, {"fortytwo", 42}, {"MAXIMUM", INT64_MAX}, {"MINIMUM", INT64_MIN}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].number);
    }
}
