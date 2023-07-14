// SPDX-License-Identifier: MIT

#include <stdio.h>

#include "elosKmsgMapperInit_utest.h"

int elosTestElosKmsgMapperInitSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperInitSuccessTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosKmsgMapperInitSuccess(UNUSED void **state) {
    SHOULD("successfully get a boot time!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    char *buf = "701900.81 10932447.52\n";
    FILE *file = fmemopen(buf, strlen(buf), "r");

    expect_string(__wrap_fopen, filename, "/proc/uptime");
    expect_string(__wrap_fopen, mode, "r");
    will_return(__wrap_fopen, file);
    MOCK_FUNC_AFTER_CALL(fopen, 0);

    expect_value(__wrap_time, timer, NULL);
    will_return(__wrap_time, 1684930154);
    MOCK_FUNC_AFTER_CALL(time, 0);

    safuResultE_t result = elosKmsgMapperInit(&mapper);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(mapper.bootTime.tv_sec, 1684228253);
    assert_int_equal(mapper.bootTime.tv_nsec, 190000000);
}
