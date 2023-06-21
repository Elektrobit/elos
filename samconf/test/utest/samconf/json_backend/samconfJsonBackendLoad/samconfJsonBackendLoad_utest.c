// SPDX-License-Identifier: MIT
#include "samconfJsonBackendLoad_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(samconfJsonBackendLoadUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(samconfTestSamconfJsonBackendLoadSuccess),
        TEST_CASE(samconfTestSamconfJsonBackendLoadSuccessArrayNoloop),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorCompromisedConfig),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigObject),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorCompromisedSubconfigArray),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorFailedAddObject),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorFailedAddArray),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorNullObjectArray),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorNullObjectObject),
        TEST_CASE(samconfTestSamconfJsonBackendLoadErrorNullBackend),
    };

    return RUN_TEST_SUITE(tests, samconfJsonBackendLoadUtest);
}

static int samconfJsonBackendLoadUtestSetup(UNUSED void **state) {
    return 0;
}

static int samconfJsonBackendLoadUtestTeardown(UNUSED void **state) {
    return 0;
}
