// SPDX-License-Identifier: MIT
#pragma once

#include <cmocka_extensions/cmocka_extensions.h>
#include <cmocka_mocks/mock_libc.h>

#define DIR       "/tmp"
#define SUBFOLDER "elosd"
#define FILENAME  "elosd.socket"

#define MOCK_DIR_NAME DIR "/" SUBFOLDER
#define MOCK_PATH     MOCK_DIR_NAME "/" FILENAME

TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetSocketAddressSuccessPathExist)
TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetSocketAddressSuccessCreateDir)
TEST_CASE_FUNC_PROTOTYPES(elosTestelosUnixConfigGetSocketAddressErrNoAccess)
