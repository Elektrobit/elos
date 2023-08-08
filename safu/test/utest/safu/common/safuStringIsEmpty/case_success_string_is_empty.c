// SPDX-License-Identifier: MIT

#include "cmocka_mocks/mock_libc.h"
#include "safuStringIsEmpty_utest.h"

int safuTestSafuStringIsEmptySuccessStringIsEmptySetup(UNUSED void **state) {
    return 0;
}

int safuTestSafuStringIsEmptySuccessStringIsEmptyTeardown(UNUSED void **state) {
    return 0;
}

void safuTestSafuStringIsEmptySuccessStringIsEmpty(UNUSED void **state) {
    const char *testStrings[] = {NULL, "", "  ", "	", "\t", "\n", "\f", "\r", "\v"};
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("safuStringIsEmpty");
    SHOULD("%s", "return SAFU_RESULT_OK since test string is empty");

    for (size_t i = 0; i < ARRAY_SIZE(testStrings); i++) {
        result = safuStringIsEmpty(testStrings[i]);
        assert_int_equal(result, SAFU_RESULT_OK);
    }
}
