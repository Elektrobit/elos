// SPDX-License-Identifier: MIT

#ifndef SAFU_TRANSFEREXACTLY_UTEST_H
#define SAFU_TRANSFEREXACTLY_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "safu/common.h"

void safuTestSafuTransferExactlyErrFdLtZero(void **state);
void safuTestSafuTransferExactlyErrBuffNull(void **state);
void safuTestSafuTransferExactlyErrLenLtOne(void **state);
void safuTestSafuTransferExactlyErrSendBytesLtZero(void **state);
void safuTestSafuTransferExactlyErrSendBytesEqZero(void **state);
void safuTestSafuTransferExactlySuccessInOneLoop(void **state);
void safuTestSafuTransferExactlySuccessInTwoLoops(void **state);
void safuTestSafuTransferExactlySuccessEintr(void **state);

#endif /* SAFU_TRANSFEREXACTLY_UTEST_H */
