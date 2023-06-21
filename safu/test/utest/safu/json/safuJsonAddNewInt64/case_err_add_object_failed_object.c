// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonAddNewInt64_utest.h"

SETUP(safuTestSafuJsonAddNewInt64ErrAddObjectFailedObject)
TEARDOWN(safuTestSafuJsonAddNewInt64ErrAddObjectFailedObject)

static void _testCase(struct json_object *jobj, const char *name, int64_t number) {
    PARAM("name = %s", name);
    PARAM("val = %ld", number);

    struct json_object *jadd = MOCK_FUNC_REAL(json_object_new_int64)(0);

    MOCK_FUNC_AFTER_CALL(json_object_new_int64, 0);

    expect_value(__wrap_json_object_new_int64, i, number);
    will_return(__wrap_json_object_new_int64, jadd);

    expect_value(safuJsonAddObject, jobj, jobj);
    expect_value(safuJsonAddObject, name, name);
    expect_value(safuJsonAddObject, jdata, jadd);
    will_return(safuJsonAddObject, -1);

    struct json_object *jint64 = safuJsonAddNewInt64(jobj, name, number);
    assert_null(jint64);
}

void safuTestSafuJsonAddNewInt64ErrAddObjectFailedObject(void **state) {
    TEST("safuJsonAddNewInt64");
    SHOULD("%s", "fail to add an int64_t to a JSON object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *name;
        int64_t number;
    } testRows[] = {
        {"NULL", 0},
        {"fortytwo", 42},
        {"MAXIMUM", INT64_MAX},
        {"MINIMUM", INT64_MIN},
    };

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name, testRows[i].number);
    }
}
