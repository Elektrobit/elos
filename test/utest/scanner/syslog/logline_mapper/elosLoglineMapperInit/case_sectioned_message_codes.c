// SPDX-License-Identifier: MIT
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

#include "elos/rpnfilter/rpnfilter.h"
#include "elosLoglineMapperInit_utest.h"
#include "logline_mapper.h"

typedef struct elosMessageCodeFilterMapping {
    elosEventMessageCodeE_t messageCode;
    elosRpnFilter_t filter;
} elosMessageCodeFilterMapping_t;

int elosTestElosLoglineMapperInitSectionedMessageCodesSetup(void **state) {
    elosLoglineMapperInitUtestInit(state);
    return 0;
}

int elosTestElosLoglineMapperInitSectionedMessageCodesTeardown(void **state) {
    elosLoglineMapperInitUtestCleanUp(state);
    return 0;
}

void elosTestElosLoglineMapperInitSectionedMessageCodes(void **state) {
    elosUteststateT_t *testState = *state;

    samconfConfig_t *crinitChildren[] = {
        &testState->childrenData[6],  // "2000": ".event.source.appName 'crinit' STRCMP"
    };
    samconfConfig_t *loginChildren[] = {
        &testState->childrenData[7],  // "1000": ".event.source.appName 'login' STRCMP"
    };

    testState->childrenData[3].children = crinitChildren;
    testState->childrenData[3].childCount = 1;

    testState->childrenData[4].children = loginChildren;
    testState->childrenData[4].childCount = 1;

    samconfConfig_t *messageCodesChildren[] = {
        &testState->childrenData[5],  // "4000": ".event.source.appName 'ssh' STRCMP"
        &testState->childrenData[3],  // "crinit": { ... },
        &testState->childrenData[4],  // "login": { ... }
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
    //              "4000": ".event.source.appName 'ssh' STRCMP"
    //              "crinit": {
    //                  "2000": ".event.source.appName 'crinit' STRCMP"
    //              },
    //              "login": {
    //                  "1000": ".event.source.appName 'login' STRCMP"
    //              }
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

    TEST("elosLoglineMapper");
    SHOULD("%s", "skip initializing all the fields in the old config format");

    result = elosLoglineMapperInit(&mapper, &root);

    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(mapper.filterList.elementCount, 1);
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
