// SPDX-License-Identifier: MIT

#include "elosEventVectorFromJsonArray_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(_testSuite)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosEventVectorFromJsonArrayErrArray),
        TEST_CASE(elosTestElosEventVectorFromJsonArrayErrVector),
        TEST_CASE(elosTestElosEventVectorFromJsonArrayErrParam),
        TEST_CASE(elosTestElosEventVectorFromJsonArraySuccess),
    };

    return RUN_TEST_SUITE(tests, _testSuite);
}

static int _testSuiteSetup(void **state) {
    elosUnitTestState_t *test;

    test = safuAllocMem(NULL, sizeof(elosUnitTestState_t));
    assert_non_null(test);
    memset(test, 0, sizeof(elosUnitTestState_t));

    test->jEventArray = json_tokener_parse("[{\"messageCode\":7000},{\"messageCode\":8000}]");
    assert_non_null(test->jEventArray);

    test->jEventArrayEmpty = json_object_new_array_ext(0);
    assert_non_null(test->jEventArrayEmpty);

    test->jEventArrayInvalid = json_object_new_object();
    assert_non_null(test->jEventArrayInvalid);

    *state = test;

    return 0;
}

static int _testSuiteTeardown(void **state) {
    elosUnitTestState_t *test = *(elosUnitTestState_t **)state;

    json_object_put(test->jEventArray);
    json_object_put(test->jEventArrayEmpty);
    json_object_put(test->jEventArrayInvalid);
    free(*state);

    return 0;
}
