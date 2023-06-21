// SPDX-License-Identifier: MIT
#pragma once

#include <safu/result.h>
#include <samconf/samconf_types.h>

#include "elos/eventfilter/eventfilter.h"

safuResultE_t elosBlacklistInitialize(elosEventFilter_t *blacklist, samconfConfig_t const *const config);
safuResultE_t elosBlacklistDelete(elosEventFilter_t *blacklist);
