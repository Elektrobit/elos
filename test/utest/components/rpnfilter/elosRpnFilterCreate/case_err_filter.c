// SPDX-License-Identifier: MIT

#include "elosRpnFilterCreate_utest.h"

int elos_testElosRpnFilterCreateErrFilterSetup(UNUSED void **state) {
    return 0;
}

void elos_testElosRpnFilterCreateErrFilter(UNUSED void **state) {
    elosRpnFilterResultE_t result;
    elosRpnFilter_t filter = ELOS_RPNFILTER_INIT;

    char const *brokenFilterList[] = {
        "AND",
        "1 AND OR",
        "1 1 EQ AND",
    };

    TEST("elosRpnFilterCreate");
    SHOULD("return SAFU_RESULT_FAILED with erroneous filters");

    for (size_t i = 0; i < ARRAY_SIZE(brokenFilterList); i += 1) {
        elosRpnFilterParam_t param = {.filterString = brokenFilterList[i]};

        PARAM("with .filterString: '%s'", param.filterString);
        result = elosRpnFilterCreate(&filter, &param);
        assert_int_equal(result, RPNFILTER_RESULT_ERROR);
    }
}

int elos_testElosRpnFilterCreateErrFilterTeardown(UNUSED void **state) {
    return 0;
}
