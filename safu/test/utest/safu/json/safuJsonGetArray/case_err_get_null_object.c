// SPDX-License-Identifier: MIT
#include "safuJsonGetArray_utest.h"

SETUP(safuTestSafuJsonGetArrayErrGetNullObject)
TEARDOWN(safuTestSafuJsonGetArrayErrGetNullObject)

static void _testCase(const struct json_object *jobj, const char *name) {
    struct json_object *resultArray = safuJsonGetArray(jobj, name, 0, NULL);

    assert_null(resultArray);
}

void safuTestSafuJsonGetArrayErrGetNullObject(void **state) {
    TEST("safuJsonGetArray");
    SHOULD("%s", "fail to get a json array from a json array by providing a string key");

    *(struct json_object **)state = json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;

    struct {
        char *name;
        size_t len;
        struct json_object *array;
    } testRows[] = {{"empty", 0, NULL}, {"one", 1, NULL}, {"short", 17, NULL}, {"longer", 50, NULL}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        testRows[i].array = json_object_new_array();
        for (size_t len = 0; len < testRows[i].len; len++) {
            safuJsonAddNewObject(testRows[i].array, NULL);
        }
        safuJsonAddObject(jobj, NULL, testRows[i].array);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].name);
    }
}
