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

#define _TEST_CONFIG \
    "{ \
        \"Config\": { \
            \"MappingRules\": { \
                \"MessageCodes\": { \
                    \"4000\": \".event.source.appName 'ssh' STRCMP\", \
                    \"ssh\": { \
                        \"5000\": { \
                            \"crinit\": { \
                                \"login\": { \
                                    \"2000\": \".event.source.appName 'crinit' STRCMP\", \
                                    \"1000\": \".event.source.appName 'login' STRCMP\" \
                                } \
                            } \
                        } \
                    } \
                } \
            } \
        } \
    }"

int elosTestElosLoglineMapperInitErrDeepNestedMessageCodesSetup(void **state) {
    elosLoglineMapperInitUtestInit(state, _TEST_CONFIG);
    return 0;
}

int elosTestElosLoglineMapperInitErrDeepNestedMessageCodesTeardown(void **state) {
    elosLoglineMapperInitUtestCleanUp(state);
    return 0;
}

void elosTestElosLoglineMapperInitErrDeepNestedMessageCodes(void **state) {
    samconfConfig_t *root = *(samconfConfig_t **)state;

    elosRpnFilter_t *resultFilter;
    elosLoglineMapper_t mapper;
    safuResultE_t result = SAFU_RESULT_FAILED;

    TEST("elosLoglineMapper");
    SHOULD("%s", "initialize with a nested config, skipping all fields not directly under \"MessageCodes\"");

    result = elosLoglineMapperInit(&mapper, root);

    assert_int_equal(result, SAFU_RESULT_OK);

    assert_int_equal(mapper.filterList.elementCount, 1);
    for (u_int32_t i = 0; i < mapper.filterList.elementCount; i++) {
        elosMessageCodeFilterMapping_t *resultMapping;

        resultMapping = (elosMessageCodeFilterMapping_t *)safuVecGet(&mapper.filterList, i);
        resultFilter = &resultMapping->filter;
        assert_int_equal(resultFilter->state, RPNFILTER_FINALIZED);
    }

    elosLoglineMapperDeleteMembers(&mapper);
}
