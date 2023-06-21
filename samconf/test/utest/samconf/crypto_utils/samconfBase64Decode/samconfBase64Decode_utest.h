// SPDX-License-Identifier: MIT
#ifndef SAMCONF_BAS64_DECODE_UTEST_H
#define SAMCONF_BAS64_DECODE_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "samconf/crypto_utils.h"
#include "samconf/samconf.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfBase64DecodeSuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfBase64DecodeAllocError)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfBase64DecodeDecodeError)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfBase64DecodePaddingError)

#endif /* SAMCONF_BAS64_DECODE_UTEST_H */
