// SPDX-License-Identifier: MIT
#include "elosLoglineMapperDoMapping_utest.h"

#include <samconf/test_utils.h>

TEST_SUITE_FUNC_PROTOTYPES(elosLoglineMapperDoMappingUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoglineMapperDoMappingSuccess),
    };

    return RUN_TEST_SUITE(tests, elosLoglineMapperDoMappingUtest);
}

int elosLoglineMapperDoMappingUtestInit(void **state, const char *configStr) {
    elosTestState_t *testState = calloc(1, sizeof(elosTestState_t));
    assert_non_null(testState);

    samconfConfigStatusE_t confRes = samconfUtilCreateMockConfigFromStr(configStr, false, &testState->config);
    if (confRes != SAMCONF_CONFIG_OK) {
        return -1;
    }
    safuResultE_t result = elosLoglineMapperInit(&testState->mapper, &testState->config);
    if (result != SAFU_RESULT_OK) {
        return -1;
    }
    testState->timezone = getenv("TZ");
    print_message("Got init done\n");
    setenv("TZ", "UTC", 1);
    tzset();

    *state = testState;
    return 0;
}

int elosLoglineMapperDoMappingUtestCleanUp(void **state) {
    elosTestState_t *testState = *state;
    samconfConfigDeleteMembers(&testState->config);
    elosLoglineMapperDeleteMembers(&testState->mapper);
    free(testState);
    state = NULL;
    return 0;
}

static int elosLoglineMapperDoMappingUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLoglineMapperDoMappingUtestTeardown(UNUSED void **state) {
    return 0;
}
