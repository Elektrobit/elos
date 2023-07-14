// SPDX-License-Identifier: MIT

#include "elosKmsgMapperInit_utest.h"

int elosTestElosKmsgMapperInitErrOpenProcSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperInitErrOpenProcTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosKmsgMapperInitErrOpenProc(UNUSED void **state) {
    SHOULD("fail to open /proc/uptime!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    expect_string(__wrap_fopen, filename, "/proc/uptime");
    expect_string(__wrap_fopen, mode, "r");
    will_return(__wrap_fopen, NULL);
    MOCK_FUNC_AFTER_CALL(fopen, 0);

    safuResultE_t result = elosKmsgMapperInit(&mapper);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    assert_int_equal(mapper.bootTime.tv_sec, 0);
    assert_int_equal(mapper.bootTime.tv_nsec, 0);
}
