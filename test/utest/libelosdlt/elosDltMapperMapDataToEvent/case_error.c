// SPDX-License-Identifier: MIT

#include <bits/types/struct_iovec.h>
#include <safu/defines.h>
#include <safu/result.h>

#include "elos/libelosdlt/dltmapper.h"
#include "elosDltMapperMapDataToEvent_utest.h"

int elosTestElosDltMapperMapDataToEventErrorSetup(UNUSED void **state) {
    return 0;
}

int elosTestElosDltMapperMapDataToEventErrorTeardown(UNUSED void **state) {
    return 0;
}

void elosTestElosDltMapperMapDataToEventError(UNUSED void **state) {
    safuResultE_t result;
    elosDltData_t testDltData = {0};
    elosEvent_t testEvent = {0};
    elosDltMapper_t testMapper = {0};

    TEST("elosDltMapperMapDataToEvent");
    SHOULD("%s", "convert dlt data into elos event fails");

    PARAM("Mapper Is NULL");
    result = elosDltMapperMapDataToEvent(NULL, &testDltData, &testEvent);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("Dltdata Is NULL");
    result = elosDltMapperMapDataToEvent(&testMapper, NULL, &testEvent);
    assert_int_equal(result, SAFU_RESULT_FAILED);

    PARAM("Event Is NULL");
    result = elosDltMapperMapDataToEvent(&testMapper, &testDltData, NULL);
    assert_int_equal(result, SAFU_RESULT_FAILED);
}
