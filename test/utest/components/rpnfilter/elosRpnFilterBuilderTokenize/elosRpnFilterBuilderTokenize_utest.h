// SPDX-License-Identifier: MIT
#ifndef ELOS_RPNFILTERCREATETOKENIZE_UTEST_H
#define ELOS_RPNFILTERCREATETOKENIZE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <string.h>

#include "elos/rpnfilter/builder.h"
#include "safu/log.h"

#define MOCK_FUNC_AFTER_CALL(METHODE, COUNT) fail_msg("FIX MOCK_FUNC_AFTER_CALL in %s:%d", __FILE__, __LINE__)

TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeErrInternal)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeErrParam)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeErrString)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeSuccessCommand)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeSuccessLocation)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeSuccessNumber)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeSuccessString)
TEST_CASE_FUNC_PROTOTYPES(elos_testElosRpnFilterBuilderTokenizeSuccess)

#define ELOS_TOKENTESTMAXCOUNT 32
#define ELOS_TOKENTESTENTRY(__type, __input, __str) \
    { .type = (__type), .length = strlen(__str), .string = (__str) }

typedef struct elosTokenTestEntry {
    char *input;
    uint32_t tokenCount;
    elosRpnFilterBuilderTokenEntry_t token[ELOS_TOKENTESTMAXCOUNT];
} elosTokenTestEntry_t;

typedef struct _TestState {
    elosRpnFilterBuilder_t *data;
} elosTestState_t;

const char *elosTokenTypeToString(elosRpnFilterBuilderTokenType_t);
elosRpnFilterResultE_t elosTokenTestEntries(elosRpnFilterBuilder_t **data, const elosTokenTestEntry_t *entry,
                                            size_t elements);

#endif /* ELOS_RPNFILTERCREATETOKENIZE_UTEST_H */
