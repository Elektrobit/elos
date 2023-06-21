// SPDX-License-Identifier: MIT
#include "cmocka_mocks/mock_jsonc.h"
#include "safuJsonGetUint64_utest.h"

SETUP(safuTestSafuJsonGetUint64ErrGetUint64FailedObject)
TEARDOWN(safuTestSafuJsonGetUint64ErrGetUint64FailedObject)

static void _testCase(struct json_object *jobj, const char *key, uint64_t val) {
    PARAM("key = \"%s\"", key);
    PARAM("val = %lu", val);
    uint64_t initResult = 117;
    uint64_t result = initResult;

    MOCK_FUNC_AFTER_CALL(json_object_get_uint64, 0);

    expect_any(__wrap_json_object_get_uint64, obj);
    will_set_errno(__wrap_json_object_get_uint64, EINVAL);
    will_return(__wrap_json_object_get_uint64, val);

    int retval = safuJsonGetUint64(jobj, key, 0, &result);

    assert_int_equal(retval, -1);
    assert_int_equal(result, initResult);
}

void safuTestSafuJsonGetUint64ErrGetUint64FailedObject(void **state) {
    TEST("safuJsonGetUint64");
    SHOULD("%s", "return -1 when json_object_get_int fails and sets errno on a json object");

    *(struct json_object **)state = json_object_new_object();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    struct {
        char *key;
        uint64_t val;
    } testRows[] = {{"Zero", 0}, {"fortytwo", 42}, {"Max", UINT64_MAX}};

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        safuJsonAddNewUint64(jobj, testRows[i].key, testRows[i].val);
    }

    for (size_t i = 0; i < ARRAY_SIZE(testRows); i++) {
        _testCase(jobj, testRows[i].key, testRows[i].val);
    }
}
