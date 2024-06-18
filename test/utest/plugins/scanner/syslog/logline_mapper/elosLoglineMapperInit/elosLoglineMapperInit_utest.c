// SPDX-License-Identifier: MIT
#include "elosLoglineMapperInit_utest.h"

#include <samconf/test_utils.h>

TEST_SUITE_FUNC_PROTOTYPES(elosLoglineMapperInitUtest)

int main() {
    const struct CMUnitTest tests[] = {
        TEST_CASE(elosTestElosLoglineMapperInitSuccess),
        TEST_CASE(elosTestElosLoglineMapperInitSectionedMessageCodes),
        TEST_CASE(elosTestElosLoglineMapperInitErrDeepNestedMessageCodes),
        TEST_CASE(elosTestElosLoglineMapperInitNullConfig),
    };

    return RUN_TEST_SUITE(tests, elosLoglineMapperInitUtest);
}

int elosLoglineMapperInitUtestInit(void **state, const char *config) {
    samconfConfig_t *testConfig = calloc(1, sizeof(samconfConfig_t));
    samconfUtilCreateMockConfigFromStr(config, false, testConfig);
    *state = testConfig;
    return 0;
}

int elosLoglineMapperInitUtestCleanUp(void **state) {
    samconfConfig_t *testConfig = *(samconfConfig_t **)state;
    samconfConfigDelete(testConfig);
    state = NULL;
    return 0;
}

static int elosLoglineMapperInitUtestSetup(UNUSED void **state) {
    return 0;
}

static int elosLoglineMapperInitUtestTeardown(UNUSED void **state) {
    return 0;
}
