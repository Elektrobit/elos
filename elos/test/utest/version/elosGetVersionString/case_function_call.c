// SPDX-License-Identifier: MIT
#define _GNU_SOURCE

#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elosGetVersionString_utest.h"

void elosTestElosetVersionStringFunctionCall(UNUSED void **state) {
    const char *currentVersion = NULL;
    char *expectedVersion = NULL;
    size_t expectedVersionLength;
    regex_t reg;
    int result;

    expectedVersionLength = asprintf(&expectedVersion, "%u.%u.%u.%s", elosVersion.major, elosVersion.minor,
                                     elosVersion.micro, elosVersion.git);
    assert_int_not_equal(expectedVersionLength, -1);
    assert_non_null(expectedVersion);
    if (regcomp(&reg, REGEX, REG_EXTENDED | REG_NOSUB) != 0)
        fail_msg("Failed to compile regular expression for version test.");
    result = regexec(&reg, expectedVersion, 0, 0, 0);
    if (result != 0) {
        regfree(&reg);
        fail_msg("Expected Version Regex did not match. Version String is invalid.");
    }
    currentVersion = elosGetVersionString();
    assert_non_null(currentVersion);
    result = regexec(&reg, currentVersion, 0, 0, 0);
    regfree(&reg);
    if (result != 0) fail_msg("Generated Version Regex did not match. Version String is invalid.");

    assert_string_equal(expectedVersion, currentVersion);

    free(expectedVersion);
}
