// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint32_utest.h"

SETUP(safuTestSafuJsonGetUint32ErrGetUint64FailedObject)
TEARDOWN(safuTestSafuJsonGetUint32ErrGetUint64FailedObject)

static void _testCase(struct json_object *jobj, char *key) {
    PARAM("key = \"%s\"", key);
    uint32_t initResult = 117;
    uint32_t result = initResult;

    expect_value(safuJsonGetUint64, jobj, jobj);
    expect_value(safuJsonGetUint64, name, key);
    expect_value(safuJsonGetUint64, idx, 0);
    will_return(safuJsonGetUint64, 0);
    will_return(safuJsonGetUint64, -1);

    int retval = safuJsonGetUint32(jobj, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetUint32ErrGetUint64FailedObject(void **state) {
    TEST("safuJsonGetUint32");
    SHOULD("%s", "fail to get an uint32 from a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;

    struct {
        char *key;
        uint32_t val;
    } testRows[] = {{"Zero", 0}, {"fortytwo", 42}, {"Max", UINT32_MAX}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewUint64(jobj, testRows[i].key, testRows[i].val);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key);
    }
}
