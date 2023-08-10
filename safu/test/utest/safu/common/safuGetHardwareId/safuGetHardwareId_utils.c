// SPDX-License-Identifier: MIT
#include <cmocka_extensions/mock_extensions.h>

#include "safuGetHardwareId_utest.h"

// The contents of this file should be moved into a generic mock interface at some point

MOCK_FUNC_VAR_NEW(safuReadFileToString);
ssize_t __real_safuReadFileToString(const char *fileName, ssize_t maxLength, char **string);
ssize_t safuReadFileToString(const char *fileName, ssize_t maxLength, char **string) {
    if (MOCK_IS_ACTIVE(safuReadFileToString)) {
        check_expected_ptr(fileName);
        check_expected(maxLength);
        check_expected_ptr(string);
        return mock_type(ssize_t);
    }
    return __real_safuReadFileToString(fileName, maxLength, string);
}
