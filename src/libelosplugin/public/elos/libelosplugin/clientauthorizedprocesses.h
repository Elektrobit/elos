// SPDX-License-Identifier: MIT
#pragma once

#include <elos/common/process.h>
#include <safu/result.h>
#include <safu/vector.h>
#include <samconf/samconf.h>

__BEGIN_DECLS

safuResultE_t elosAuthorizedProcessInitialize(safuVec_t *authorizedprocessfilters, samconfConfig_t const *const config);
safuResultE_t elosAuthorizedProcessDelete(safuVec_t *authorizedprocessfilters);
bool elosAuthorizedProcessCheck(safuVec_t *authorizedprocessfilters, elosProcessIdentity_t *process);

__END_DECLS
