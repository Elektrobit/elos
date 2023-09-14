// SPDX-License-Identifier: MIT

#include <safu/common.h>

#include "elos/event/event.h"
#include "elosKmsgMapperDoMapping_utest.h"
#include "kmsg_mapper.h"

int elosTestElosKmsgMapperDoMappingErrMissingNumberSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperDoMappingErrMissingNumberTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(const elosKmsgMapper_t *mapper, char *msgLine) {
    elosEvent_t event = {0};
    PARAM("\"%s\"", msgLine);

    safuResultE_t result = elosKmsgMapperDoMapping(mapper, &event, msgLine);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    elosEventDeleteMembers(&event);
}

void elosTestElosKmsgMapperDoMappingErrMissingNumber(UNUSED void **state) {
    SHOULD("fail the mapping of kmsg because of missing numbers!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    char *testData[] = {"",
                        ",772,1864963,-;raid6: using algorithm avx2x2 gen() 54789 MB/s",
                        "3,,77273,-;x86/cpu: SGX disabled by BIOS.",
                        "3,772,,-;x86/cpu: SGX disabled by BIOS.",
                        "3,,,-;x86/cpu: SGX disabled by BIOS.",
                        "6,772,1864963,;raid6: using algorithm avx2x2 gen() 54789 MB/s",
                        "r,304,77273,-;x86/cpu: SGX disabled by BIOS.",
                        "3,fu,77273,-;x86/cpu: SGX disabled by BIOS.",
                        "3,304,b,-;x86/cpu: SGX disabled by BIOS.",
                        "184467440737095516153,304,77273,-;x86/cpu: SGX disabled by BIOS.",
                        "3,1844674407370955161534,77273,-;x86/cpu: SGX disabled by BIOS.",
                        "3,304,184467440737095516157,-;x86/cpu: SGX disabled by BIOS.",
                        "30,825,-;systemd[1]: Listening on systemd-udevd-kernel.socket - udev Kernel Socket."};
    for (size_t i = 0; i < ARRAY_SIZE(testData); i++) {
        _testCase(&mapper, testData[i]);
    }
}
