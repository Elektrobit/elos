// SPDX-License-Identifier: MIT
#include "safuJsonAddNewTimestamp_utest.h"

SETUP(safuTestSafuJsonAddNewTimestampErrNoTimespecArray)
TEARDOWN(safuTestSafuJsonAddNewTimestampErrNoTimespecArray)

static void _testCase(struct json_object *jobj, size_t idx) {
    PARAM("idx = %lu", idx);

    struct json_object *jtimestamp = safuJsonAddNewTimestamp(jobj, NULL, NULL);

    assert_null(jtimestamp);

    jtimestamp = json_object_array_get_idx(jobj, idx);

    assert_null(jtimestamp);
}

void safuTestSafuJsonAddNewTimestampErrNoTimespecArray(void **state) {
    TEST("safuJsonAddNewTimestamp");
    SHOULD("%s", "fail to provide a timestamp to add to a json array");

    *(struct json_object **)state = __real_json_object_new_array();
    struct json_object *jobj = *(struct json_object **)state;
    assert_non_null(jobj);

    for (size_t i = 0; i < 12; i++) {
        _testCase(jobj, i);
    }
}
