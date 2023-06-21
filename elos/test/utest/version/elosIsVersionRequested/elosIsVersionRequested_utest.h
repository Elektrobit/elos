// SPDX-License-Identifier: MIT
#ifndef TEST_VERSION_IS_VERSION_REQUESTED_H__
#define TEST_VERSION_IS_VERSION_REQUESTED_H__

#include <cmocka_extensions/cmocka_extensions.h>

#include "version.h"

void elosTestElossVersionRequestedEmptyRequest(void **state);
void elosTestElossVersionRequestedLongRequest(void **state);
void elosTestElossVersionRequestedNoRequest(void **state);
void elosTestElossVersionRequestedNullParam(void **state);
void elosTestElossVersionRequestedShortRequest(void **state);

#endif
