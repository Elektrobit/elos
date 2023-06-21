// SPDX-License-Identifier: MIT
#include "safuJsonGetArray_utest.h"

SETUP(safuTestSafuJsonGetArrayErrGetNullArray)
TEARDOWN(safuTestSafuJsonGetArrayErrGetNullArray)

static void _testCase(const struct json_object *jobj, size_t idx) {
    struct json_object *resultArray = safuJsonGetArray(jobj, NULL, idx, NULL);

    assert_null(resultArray);
}

void safuTestSafuJsonGetArrayErrGetNullArray(void **state) {
    TEST("safuJsonGetArray");
    SHOULD("%s", "fail to get a json array from a json object with a number index");

    *(struct json_object **)state = json_object_new_object();
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
        safuJsonAddObject(jobj, testRows[i].name, testRows[i].array);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, i);
    }
}
