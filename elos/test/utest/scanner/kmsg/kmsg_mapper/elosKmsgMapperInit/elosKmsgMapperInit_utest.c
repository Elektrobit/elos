// SPDX-License-Identifier: MIT

#include "elosKmsgMapperInit_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosKmsgMapperInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosKmsgMapperInitSuccess),
        TEST_CASE(elosTestElosKmsgMapperInitErrOpenProc),
        TEST_CASE(elosTestElosKmsgMapperInitErrFscanfFail),
    };

    return RUN_TEST_SUITE(tests, elosKmsgMapperInitUtest);
}

int elosKmsgMapperInitUtestSetup(UNUSED void **state) {
    return 0;
}
int elosKmsgMapperInitUtestTeardown(UNUSED void **state) {
    return 0;
}
