// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTEREXECUTE_UTEST_H
#define ELOS_RPNFILTEREXECUTE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <safu/log.h>

#include "elos/rpnfilter/rpnfilter.h"

#define ELOS_STACK(__t, __b, __d) \
    { .type = (__t), .bytes = (__b), .data.ptr = (__d) }
#define ELOS_STACK_I32(__d) \
    { .type = RPNFILTER_VALUE_I32, .bytes = sizeof(int32_t), .data.i32 = (__d) }
#define ELOS_STACK_I64(__d) \
    { .type = RPNFILTER_VALUE_I64, .bytes = sizeof(int64_t), .data.i64 = (__d) }

#define ELOS_MAX_ENTRIES 32

typedef struct elosStateTestExecute {
    elosRpnFilter_t filter;
} elosStateTestExecute_t;

typedef struct elosTestExecuteEntry {
    elosRpnFilterResultE_t result;
    uint32_t paramCount;
    elosRpnFilterStackEntry_t param[ELOS_MAX_ENTRIES];
} elosTestExecuteEntry_t;

typedef struct elosTestExecuteSet {
    char const *filter;
    uint32_t entryCount;
    elosTestExecuteEntry_t entry[ELOS_MAX_ENTRIES];
} elosTestExecuteSet_t;

void elosTestExecuteSet(elosStateTestExecute_t *test, elosTestExecuteSet_t const *set, size_t elements);

TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterExecuteErrParam)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterExecuteSuccessArithmetic)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterExecuteSuccessOperation)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterExecuteSuccessString)

#endif /* ELOS_RPNFILTEREXECUTE_UTEST_H */
