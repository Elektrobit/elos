// SPDX-License-Identifier: MIT

#include <cmocka_extensions/cmocka_extensions.h>

#include "elosCompleteStoragePath_utest.h"
#include "safu/mock_safu.h"

#define HARDWAREID "TESTMACHINE"

int ELOS_testelosCompleteStoragePathSuccessSetup(UNUSED void **state) {
    return 0;
}

int ELOS_testelosCompleteStoragePathSuccessTeardown(UNUSED void **state) {
    MOCK_FUNC_NEVER(safuGetHardwareId);
    return 0;
}

void ELOS_testelosCompleteStoragePathSuccess(UNUSED void **state) {
    struct {
        const char *const patternString;
        const char *const countString;
        const char *const dateString;
        const char *const expectedResult;
        bool hasCount;
    } testData[] = {
        {
            .patternString = "test_%count%_%date%_%host%.log",
            .countString = "0",
            .dateString = "2001-01-01",
            .expectedResult = "test_0_2001-01-01_" HARDWAREID ".log",
            .hasCount = true,
        },
        {
            .patternString = "test_%host%_%date%_%count%.log",
            .countString = "0",
            .dateString = "2001-01-01",
            .expectedResult = "test_" HARDWAREID "_2001-01-01_0.log",
            .hasCount = true,
        },
        {
            .patternString = "test_%date%_%count%.log",
            .countString = "0",
            .dateString = "2001-01-01",
            .expectedResult = "test_2001-01-01_0.log",
            .hasCount = true,
        },
        {
            .patternString = "test_%count%.log",
            .countString = "0",
            .dateString = "2001-01-01",
            .expectedResult = "test_0.log",
            .hasCount = true,
        },
        {
            .patternString = "test.log",
            .countString = NULL,
            .dateString = NULL,
            .expectedResult = "test.log.0",
            .hasCount = true,
        },
        {
            .patternString = "test_%count%_%count%_%count%.log",
            .countString = "42",
            .dateString = NULL,
            .expectedResult = "test_42_42_42.log",
            .hasCount = true,
        },
        {
            .patternString = "test_%date%_%date%_%date%.log",
            .countString = NULL,
            .dateString = "2001-01-01",
            .expectedResult = "test_2001-01-01_2001-01-01_2001-01-01.log.0",
            .hasCount = true,
        },
        {
            .patternString = "test_%host%_%host%_%host%.log",
            .countString = NULL,
            .dateString = NULL,
            .expectedResult = "test_" HARDWAREID "_" HARDWAREID "_" HARDWAREID ".log",
            .hasCount = false,
        },
    };

    MOCK_FUNC_ALWAYS(safuGetHardwareId);
    will_return_always(__wrap_safuGetHardwareId, HARDWAREID);

    for (size_t i = 0; i < ARRAY_SIZE(testData); i++) {
        bool hasCount = false;
        char *path1 = elosCompleteStoragePath(testData[i].patternString, strlen(testData[i].expectedResult) + 1,
                                              testData[i].countString, testData[i].dateString, &hasCount);
        assert_string_equal(path1, testData[i].expectedResult);
        assert_int_equal(hasCount, testData[i].hasCount);
        free(path1);
    }
}
