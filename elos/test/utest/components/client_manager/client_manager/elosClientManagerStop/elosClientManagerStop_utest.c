// SPDX-License-Identifier: MIT
#include "elosClientManagerStop_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerStopUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosClientManagerStopErrContextNull),
                                       TEST_CASE(elosTestElosClientManagerStopExtErrSemDestroy),
                                       TEST_CASE(elosTestElosClientManagerStopNoManagerStarted),
                                       TEST_CASE(elosTestElosClientManagerStopClientManagerActive)};
    return RUN_TEST_SUITE(tests, elosClientManagerStopUtest);
}

void elosClientManagerStopUtestInitParameters(void **state) {
    elosUteststateT_t *testState = *state;

    testState->parameters->config = calloc(1, sizeof(samconfConfig_t));
    int retval = testState->parameters->config == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    testState->parameters->eventProcessor = calloc(1, sizeof(elosEventProcessor_t));
    retval = testState->parameters->eventProcessor == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    testState->parameters->logAggregator = calloc(1, sizeof(elosLogAggregator_t));
    retval = testState->parameters->logAggregator == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
}

void elosClientManagerStopUtestDeleteParameters(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->parameters->logAggregator);
    free(testState->parameters->eventProcessor);
    free(testState->parameters->config);
}

static int elosClientManagerStopUtestSetup(void **state) {
    elosUteststateT_t *testState = safuAllocMem(NULL, sizeof(*testState));
    int retval = testState == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    testState->context = safuAllocMem(NULL, sizeof(elosClientManagerContext_t));
    retval = testState->context == NULL ? -1 : 0;
    assert_int_equal(retval, 0);
    memset(testState->context, 0, sizeof(elosClientManagerContext_t));

    testState->parameters = safuAllocMem(NULL, sizeof(elosClientManagerParam_t));
    retval = testState->parameters == NULL ? -1 : 0;
    assert_int_equal(retval, 0);

    *state = testState;
    return 0;
}

static int elosClientManagerStopUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->parameters);
    free(testState->context);
    free(testState);
    return 0;
}
