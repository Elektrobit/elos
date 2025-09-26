// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/defines.h>
#include <safu/result.h>

#include "elos/libelosdlt/dltmapper.h"
#include "elosDltMapperMapDataToEvent_utest.h"

int elosTestElosDltMapperMapDataToEventErrorDefaultValuesSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosDltMapperMapDataToEventErrorDefaultValuesTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosDltMapperMapDataToEventErrorDefaultValues(UNUSED void **state) {
    safuResultE_t result;
    unsigned char testpayload[] = "testpayload";
    elosDltData_t testDltData = {.stdHeader = {.headerType = {.wtms = true}, .timestamp = 1},
                                 .payload = {.data = testpayload, .size = 11},
                                 .dltDataSize = {.dataSize = 23}};
    elosDltMapper_t testMapper = {.fileName = "testFile",
                                  .appId = "testApp",
                                  .hardwareid = "testhwid",
                                  .pid = 0,
                                  .startTime = {.tv_sec = 1, .tv_nsec = 0}};

    TEST("elosDltMapperMapDataToEvent");
    SHOULD("%s", "fail to convert since default mapper values are null");

    PARAM("Mapper Filename Is NULL");
    testMapper.fileName = NULL;
    testMapper.appId = "testApp";
    testMapper.hardwareid = "testhwid";
    elosEvent_t testEventFilename = {0};
    result = elosDltMapperMapDataToEvent(&testMapper, &testDltData, &testEventFilename);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    result = elosEventDeleteMembers(&testEventFilename);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("Mapper AppId Is NULL");
    testMapper.fileName = "testFile";
    testMapper.appId = NULL;
    testMapper.hardwareid = "testhwid";
    elosEvent_t testEventAppID = {0};

    result = elosDltMapperMapDataToEvent(&testMapper, &testDltData, &testEventAppID);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    result = elosEventDeleteMembers(&testEventAppID);
    assert_int_equal(result, SAFU_RESULT_OK);

    PARAM("Mapper HardwareId Is NULL");
    testMapper.fileName = "testFile";
    testMapper.appId = "testApp";
    testMapper.hardwareid = NULL;
    elosEvent_t testEventAppIDHwId = {0};

    result = elosDltMapperMapDataToEvent(&testMapper, &testDltData, &testEventAppIDHwId);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    result = elosEventDeleteMembers(&testEventAppIDHwId);
    assert_int_equal(result, SAFU_RESULT_OK);
}
