// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuStringIsEmpty_utest.h"

int safuTestSafuStringIsEmptySuccessStringIsNotEmptySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuStringIsEmptySuccessStringIsNotEmptyTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuStringIsEmptySuccessStringIsNotEmpty(UNUSED void **state) {
    const char *testStrings[] = {" testString",  "teststring  ", "test	string", "test\tstring",
                                 "teststring\n", "test\fstring", "teststring\r", "test\vstring"};
    safuResultE_t result = SAFU_RESULT_OK;

    TEST("safuStringIsEmpty");
    SHOULD("%s", "return SAFU_RESULT_FAILED since test string is not empty");

    for (size_t i = 0; i < ARRAY_SIZE(testStrings); i++) {
        result = safuStringIsEmpty(testStrings[i]);
        assert_int_equal(result, SAFU_RESULT_FAILED);
    }
}
