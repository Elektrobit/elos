// SPDX-License-Identifier: MIT

#include <safu/common.h>

#include "elos/event/event.h"
#include "elosKmsgMapperDoMapping_utest.h"
#include "kmsg_mapper.h"

int elosTestElosKmsgMapperDoMappingErrEmptyMsgSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosKmsgMapperDoMappingErrEmptyMsgTeardown(UNUSED void **state) {
    return 0;
}

static void _testCase(const elosKmsgMapper_t *mapper, char *msgLine) {
    elosEvent_t event = {0};
    PARAM("\"%s\"", msgLine);

    safuResultE_t result = elosKmsgMapperDoMapping(mapper, &event, msgLine);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    elosEventDeleteMembers(&event);
}

void elosTestElosKmsgMapperDoMappingErrEmptyMsg(UNUSED void **state) {
    SHOULD("fail the kmsg mapping of missing/wrong readable message!");
    elosKmsgMapper_t mapper = {.bootTime = {.tv_sec = 0, .tv_nsec = 0}};

    char *testData[] = {"", "6,772,1864963,-raid6: using algorithm avx2x2 gen() 54789 MB/s", "30,825,2389278,-;",
                        "30,825,2389278,-"};
    for (size_t i = 0; i < ARRAY_SIZE(testData); i++) {
        _testCase(&mapper, testData[i]);
    }
}
