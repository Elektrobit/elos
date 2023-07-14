// SPDX-License-Identifier: MIT

#include "elosKmsgMapperDoMapping_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosKmsgMapperDoMappingUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosKmsgMapperDoMappingSuccess),
        TEST_CASE(elosTestElosKmsgMapperDoMappingErrEmptyMsg),
        TEST_CASE(elosTestElosKmsgMapperDoMappingErrMissingNumber),
    };

    return RUN_TEST_SUITE(tests, elosKmsgMapperDoMappingUtest);
}

int elosKmsgMapperDoMappingUtestSetup(UNUSED void **state) {
    return 0;
}
int elosKmsgMapperDoMappingUtestTeardown(UNUSED void **state) {
    return 0;
}
