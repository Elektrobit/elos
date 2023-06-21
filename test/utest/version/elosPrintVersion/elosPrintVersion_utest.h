// SPDX-License-Identifier: MIT
#ifndef __ELOS_GET_VERSION_STRING_UTEST_H__
#define __ELOS_GET_VERSION_STRING_UTEST_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "version.h"

#define VERSION_PREFIX "versiontest"
#define REGEX          "[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[a-z0-9]{4,20}"

void elosTestElosrintVersionNullPrefix(void **state);
void elosTestElosrintVersionWithPrefix(void **state);
void elosTestElosrintVersionNullDest(void **state);

#endif
