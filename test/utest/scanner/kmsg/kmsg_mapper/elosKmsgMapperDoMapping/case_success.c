// SPDX-License-Identifier: MIT

#include <safu/common.h>

#include "elos/event/event.h"
#include "elosKmsgMapperDoMapping_utest.h"
#include "kmsg_mapper.h"

int elosTestElosKmsgMapperDoMappingSuccessSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperDoMappingSuccessTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(const elosKmsgMapper_t *mapper, char *msgLine, const elosEvent_t *expectedEvent) {
    elosEvent_t event = {0};
    PARAM(
        "\"%s\" -> { .date = { .tv_sec = %lu, .tv_nsec = %lu }, .severity = %d, .classification = %lu, .messageCode = "
        "%d, "
        ".payload = \"%s\" }",
        msgLine, expectedEvent->date.tv_sec, expectedEvent->date.tv_nsec, expectedEvent->severity,
        expectedEvent->classification, expectedEvent->messageCode, expectedEvent->payload);

    safuResultE_t result = elosKmsgMapperDoMapping(mapper, &event, msgLine);
    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(event.messageCode, expectedEvent->messageCode);
    assert_int_equal(event.classification, expectedEvent->classification);
    assert_int_equal(event.severity, expectedEvent->severity);
    assert_string_equal(event.payload, expectedEvent->payload);
    assert_int_equal(event.date.tv_sec, expectedEvent->date.tv_sec);
    assert_int_equal(event.date.tv_nsec, expectedEvent->date.tv_nsec);
}

void elosTestElosKmsgMapperDoMappingSuccess(UNUSED void **state) {
    SHOULD("successfully map kmsg messages!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    elosEvent_t events[] = {
        {
            .date = {.tv_sec = 1, .tv_nsec = 864963000},
            .messageCode = ELOS_MSG_CODE_KMSG,
            .classification = ELOS_CLASSIFICATION_KERNEL,
            .severity = ELOS_SEVERITY_INFO,
            .payload = "6,772,1864963,-;raid6: using algorithm avx2x2 gen() 54789 MB/s",
        },
        {
            .date = {.tv_sec = 2, .tv_nsec = 389278000},
            .messageCode = ELOS_MSG_CODE_KMSG,
            .classification = ELOS_CLASSIFICATION_PROCESS,
            .severity = ELOS_SEVERITY_INFO,
            .payload = "30,825,2389278,-;systemd[1]: Listening on systemd-udevd-kernel.socket - udev Kernel Socket.",
        },
        {
            .date = {.tv_sec = 0, .tv_nsec = 645086000},
            .messageCode = ELOS_MSG_CODE_KMSG,
            .classification = ELOS_CLASSIFICATION_KERNEL,
            .severity = ELOS_SEVERITY_DEBUG,
            .payload = "7,565,645086,-;Monitor-Mwait will be used to enter C-1 state",
        },
        {
            .date = {.tv_sec = 0, .tv_nsec = 77273000},
            .messageCode = ELOS_MSG_CODE_KMSG,
            .classification = ELOS_CLASSIFICATION_KERNEL,
            .severity = ELOS_SEVERITY_WARN,
            .payload = "3,240,77273,-;x86/cpu: SGX disabled by BIOS.",
        }};
    for (size_t i = 0; i < ARRAY_SIZE(events); i++) {
        _testCase(&mapper, events[i].payload, &events[i]);
    }
}
