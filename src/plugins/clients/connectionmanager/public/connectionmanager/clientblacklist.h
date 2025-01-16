// SPDX-License-Identifier: MIT
#pragma once

#include <elos/eventfilter/eventfilter.h>
#include <safu/result.h>
#include <samconf/samconf_types.h>

__BEGIN_DECLS

safuResultE_t elosBlacklistInitialize(elosEventFilter_t *blacklist, samconfConfig_t const *const config);
safuResultE_t elosBlacklistDelete(elosEventFilter_t *blacklist);

__END_DECLS
