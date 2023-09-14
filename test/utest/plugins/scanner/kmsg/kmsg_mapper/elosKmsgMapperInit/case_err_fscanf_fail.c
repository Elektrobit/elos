// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "elosKmsgMapperInit_utest.h"

int elosTestElosKmsgMapperInitErrFscanfFailSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperInitErrFscanfFailTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosKmsgMapperInitErrFscanfFail(UNUSED void **state) {
    SHOULD("fail to scan all the values from /proc/uptime!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    char *buf = "701900.stuff81 10932447.52\n";
    FILE *file = fmemopen(buf, strlen(buf), "r");

    expect_string(__wrap_fopen, filename, "/proc/uptime");
    expect_string(__wrap_fopen, mode, "r");
    will_return(__wrap_fopen, file);
    MOCK_FUNC_AFTER_CALL(fopen, 0);

    safuResultE_t result = elosKmsgMapperInit(&mapper);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    assert_int_equal(mapper.bootTime.tv_sec, 0);
    assert_int_equal(mapper.bootTime.tv_nsec, 0);
}
