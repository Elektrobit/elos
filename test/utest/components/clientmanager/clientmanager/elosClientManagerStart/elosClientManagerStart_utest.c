// SPDX-License-Identifier: MIT
#include "elosClientManagerStart_utest.h"

TEST_SUITE_FUNC_PROTOTYPES(elosClientManagerStartUtest)

int main() {
    const struct CMUnitTest tests[] = {TEST_CASE(elosTestElosClientManagerStartErrContextNull),
                                       TEST_CASE(elosTestElosClientManagerStartErrParametersNull),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrInetPton),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrContextMutexInit),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrSemInit),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrConnectionMutexInitFirst),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrConnectionMutexInitMid),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrConnectionMutexInitLast),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrSocket),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrSetsockopt),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrBind),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrListen),
                                       TEST_CASE(elosTestElosClientManagerStartExtErrPthreadCreate),
                                       TEST_CASE(elosTestElosClientManagerStartSuccess),
                                       TEST_CASE(elosTestElosClientManagerStartErrBlacklistInitialize)};
    return RUN_TEST_SUITE(tests, elosClientManagerStartUtest);
}

void elosClientManagerStartUtestInitParameters(void **state) {
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

void elosClientManagerStartUtestDeleteParameters(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->parameters->logAggregator);
    free(testState->parameters->eventProcessor);
    free(testState->parameters->config);
}

static int elosClientManagerStartUtestSetup(void **state) {
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

static int elosClientManagerStartUtestTeardown(void **state) {
    elosUteststateT_t *testState = *state;
    free(testState->parameters);
    free(testState->context);
    free(testState);
    return 0;
}
