// SPDX-License-Identifier: MIT
#include <samconf/samconf.h>
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

int elosTestElosLoglineMapperInitErrDeepNestedMessageCodesSetup(void **state) {
    elosLoglineMapperInitUtestInit(state);
    return 0;
}

int elosTestElosLoglineMapperInitErrDeepNestedMessageCodesTeardown(void **state) {
    elosLoglineMapperInitUtestCleanUp(state);
    return 0;
}

void elosTestElosLoglineMapperInitErrDeepNestedMessageCodes(void **state) {
    elosUteststateT_t *testState = *state;

    samconfConfig_t *loginChildren[] = {
        &testState->childrenData[6],  // "2000": ".event.source.appName 'crinit' STRCMP"
        &testState->childrenData[7],  // "1000": ".event.source.appName 'login' STRCMP"
    };
    testState->childrenData[4].children = loginChildren;
    testState->childrenData[4].childCount = ARRAY_SIZE(loginChildren);

    samconfConfig_t *crinitChildren[] = {
        &testState->childrenData[4],  // "login": { ... }
    };
    testState->childrenData[3].children = crinitChildren;
    testState->childrenData[3].childCount = ARRAY_SIZE(crinitChildren);

    samconfConfig_t *numberChildren[] = {
        &testState->childrenData[3],  // "crinit": { ... }
    };
    testState->childrenData[8].children = numberChildren;
    testState->childrenData[8].childCount = ARRAY_SIZE(numberChildren);

    samconfConfig_t *sshChildren[] = {
        &testState->childrenData[8],  // "5000": { ... }
    };
    testState->childrenData[2].children = sshChildren;
    testState->childrenData[2].childCount = ARRAY_SIZE(sshChildren);

    samconfConfig_t *messageCodesChildren[] = {
        &testState->childrenData[5],  // "4000": ".event.source.appName 'ssh' STRCMP",
        &testState->childrenData[2],  // "ssh": { ... },
    };
    testState->childrenData[1].children = messageCodesChildren;
    testState->childrenData[1].childCount = ARRAY_SIZE(messageCodesChildren);

    samconfConfig_t *mappingRulesChildren[] = {
        &testState->childrenData[1],
    };

    testState->childrenData[0].children = mappingRulesChildren;
    testState->childrenData[0].childCount = ARRAY_SIZE(mappingRulesChildren);

    samconfConfig_t *syslogScannerChildren[] = {
        &testState->childrenData[0],
    };

    //  "SyslogScannere": {
    //      "MappingRules": {
    //          "MessageCodes": {
    //              "4000": ".event.source.appName 'ssh' STRCMP",
    //              "ssh": {
    //                  "5000": {
    //                      "crinit": {
    //                          "login": {
    //                              "2000": ".event.source.appName 'crinit' STRCMP",
    //                              "1000": ".event.source.appName 'login' STRCMP"
    //                          }
    //                      }
    //                  }
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
    SHOULD("%s", "initialize with a nested config, skipping all fields not directly under \"MessageCodes\"");

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
