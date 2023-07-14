// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTERBUILDERFINALIZE_UTEST_H
#define ELOS_RPNFILTERBUILDERFINALIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "elos/rpnfilter/builder.h"
#include "safu/log.h"

#define ELOS_STEPTESTMAXCOUNT  16
#define ELOS_STACKTESTMAXCOUNT 16

#define _STEP(__t) \
    { .type = (__t), .token = NULL }
#define _STACK(__t, __b, __d) \
    { .type = (__t), .bytes = (__b), .data.ptr = (__d) }
#define _STACK_U32(__d) \
    { .type = RPNFILTER_VALUE_U32, .bytes = sizeof(uint32_t), .data.u32 = (__d) }
#define _STACK_I32(__d) \
    { .type = RPNFILTER_VALUE_I32, .bytes = sizeof(int32_t), .data.i32 = (__d) }
#define _STACK_U64(__d) \
    { .type = RPNFILTER_VALUE_U64, .bytes = sizeof(uint64_t), .data.u64 = (__d) }
#define _STACK_I64(__d) \
    { .type = RPNFILTER_VALUE_I64, .bytes = sizeof(int64_t), .data.i64 = (__d) }

typedef struct elosTestFinalizeEntry {
    char *input;
    uint32_t stepCount;
    uint32_t stackCount;
    elosRpnFilterBuilderStepEntry_t step[ELOS_STEPTESTMAXCOUNT];
    elosRpnFilterStackEntry_t stack[ELOS_STACKTESTMAXCOUNT];
} elosTestFinalizeEntry_t;

typedef struct elosTestFinalize {
    elosRpnFilterBuilder_t *data;
    elosRpnFilter_t *filter;
    elosTestFinalizeEntry_t const *entry;
    uint32_t entryCount;
} elosTestFinalize_t;

elosRpnFilterResultE_t elosVerifyFinalize(elosTestFinalize_t *test);

TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderFinalizeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderFinalizeSuccess)

#endif /* ELOS_RPNFILTERBUILDERFINALIZE_UTEST_H */
