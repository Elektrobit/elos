// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "elos/rpnfilter/rpnfilter.h"
#include "elosLoglineMapperInit_utest.h"
#include "logline_mapper.h"
#include "safu/common.h"

typedef struct elosMessageCodeFilterMapping {
    elosEventMessageCodeE_t messageCode;
    elosRpnFilter_t filter;
} elosMessageCodeFilterMapping_t;

int elosTestElosLoglineMapperInitSuccessSetup(void **state) {
    elosLoglineMapperInitUtestInit(state);
    return 0;
}

int elosTestElosLoglineMapperInitSuccessTeardown(void **state) {
    elosLoglineMapperInitUtestCleanUp(state);
    return 0;
}

void elosTestElosLoglineMapperInitSuccess(void **state) {
    elosUteststateT_t *testState = *state;

    samconfConfig_t *messageCodesChildren[] = {
        &testState->childrenData[5],  // "4000": ".event.source.appName 'ssh' STRCMP",
        &testState->childrenData[6],  // "2000": ".event.source.appName 'crinit' STRCMP",
        &testState->childrenData[7],  // "1000": ".event.source.appName 'login' STRCMP"
    };

    testState->childrenData[1].children = messageCodesChildren;
    testState->childrenData[1].childCount = 3;

    samconfConfig_t *mappingRulesChildren[] = {
        &testState->childrenData[1],
    };

    testState->childrenData[0].children = mappingRulesChildren;
    testState->childrenData[0].childCount = 1;

    samconfConfig_t *syslogScannerChildren[] = {
        &testState->childrenData[0],
    };

    //  "SyslogScannere": {
    //      "MappingRules": {
    //          "MessageCodes": {
    //               "4000": ".event.source.appName 'ssh' STRCMP",
    //               "2000": ".event.source.appName 'crinit' STRCMP",
    //               "1000": ".event.source.appName 'login' STRCMP"
    //          }
    //      }
    //  }
    samconfConfig_t root = {
        .parent = NULL,
        .key = "SyslogScanner",
        .type = SAMCONF_CONFIG_VALUE_OBJECT,
        .children = syslogScannerChildren,
        .childCount = 1,
    };

    elosRpnFilter_t *resultFilter;
    elosLoglineMapper_t mapper;
    safuResultE_t result = SAFU_RESULT_FAILED;
    size_t filterIndex = 0;

    TEST("ElosLoglineMapper");
    SHOULD("%s", "initialize a mapper with given, correctly formated, config");

    result = elosLoglineMapperInit(&mapper, &root);

    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(mapper.filterList.elementCount, 3);
    for (u_int32_t i = 0; i < mapper.filterList.elementCount; i++) {
        elosMessageCodeFilterMapping_t *resultMapping;

        resultMapping = (elosMessageCodeFilterMapping_t *)safuVecGet(&mapper.filterList, i);
        resultFilter = &resultMapping->filter;
        assert_non_null(
            strstr(testState->childrenData[5 + filterIndex].value.string, resultFilter->values->entry[1].data.str));
        ++filterIndex;
    }

    elosLoglineMapperDeleteMembers(&mapper);
}
