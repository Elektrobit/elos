// SPDX-License-Identifier: MIT
#ifndef SAMCONF_LOAD_PUBLIC_KEY_UTEST_H
#define SAMCONF_LOAD_PUBLIC_KEY_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>

#include "samconf/crypto_utils.h"
#include "samconf/samconf.h"

TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfLoadPublicKeySuccess)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfLoadPublicKeyNullPathError)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfLoadPublicKeyFopenError)
TEST_CASE_FUNC_PROTOTYPES(samconfTestSamconfLoadPublicKeyPEMReadError)

#endif /* SAMCONF_LOAD_PUBLIC_KEY_UTEST_H */
