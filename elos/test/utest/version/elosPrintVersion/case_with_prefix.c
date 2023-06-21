// SPDX-License-Identifier: MIT
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "elosPrintVersion_utest.h"

void elosTestElosrintVersionWithPrefix(UNUSED void **state) {
    FILE *expectedStream = NULL;
    FILE *currentStream = NULL;
    char *expectedVersion = NULL;
    char *currentVersion = NULL;
    size_t expectedVersionLen = 0;
    size_t currentVersionLen = 0;
    regex_t reg;
    int result;

    expectedStream = open_memstream(&expectedVersion, &expectedVersionLen);
    assert_non_null(expectedStream);
    fprintf(expectedStream, VERSION_PREFIX "-%u.%u.%u.%s", elosVersion.major, elosVersion.minor, elosVersion.micro,
            elosVersion.git);
    fflush(expectedStream);
    if (regcomp(&reg, VERSION_PREFIX "-" REGEX, REG_EXTENDED | REG_NOSUB) != 0) {
        fail_msg("Failed to compile regular expression for version test.");
    }
    result = regexec(&reg, expectedVersion, 0, 0, 0);
    if (result != 0) {
        regfree(&reg);
        fail_msg("Expected Version Regex did not match. Version String is invalid.");
    }

    currentStream = open_memstream(&currentVersion, &currentVersionLen);
    assert_non_null(currentStream);
    elosPrintVersion(currentStream, VERSION_PREFIX);
    fflush(currentStream);
    result = regexec(&reg, currentVersion, 0, 0, 0);
    regfree(&reg);
    if (result != 0) {
        fail_msg("Expected Version Regex did not match. Version String is invalid.");
    }

    assert_true(expectedVersionLen == currentVersionLen);
    assert_string_equal(currentVersion, expectedVersion);

    fclose(expectedStream);
    fclose(currentStream);
    free(expectedVersion);
    free(currentVersion);
}
