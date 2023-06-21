// SPDX-License-Identifier: MIT
#include "safuTransferExactly_utest.h"

int main() {
    const struct CMUnitTest tests[] = {cmocka_unit_test(safuTestSafuTransferExactlyErrFdLtZero),
                                       cmocka_unit_test(safuTestSafuTransferExactlyErrBuffNull),
                                       cmocka_unit_test(safuTestSafuTransferExactlyErrLenLtOne),
                                       cmocka_unit_test(safuTestSafuTransferExactlyErrSendBytesLtZero),
                                       cmocka_unit_test(safuTestSafuTransferExactlyErrSendBytesEqZero),
                                       cmocka_unit_test(safuTestSafuTransferExactlySuccessInOneLoop),
                                       cmocka_unit_test(safuTestSafuTransferExactlySuccessInTwoLoops),
                                       cmocka_unit_test(safuTestSafuTransferExactlySuccessEintr)};

    return cmocka_run_group_tests(tests, NULL, NULL);
}
