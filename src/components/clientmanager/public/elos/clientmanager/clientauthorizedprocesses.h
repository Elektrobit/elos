// SPDX-License-Identifier: MIT
#pragma once

#include <samconf/samconf.h>

#include "elos/clientmanager/clientauthorization.h"
#include "elos/common/process.h"
#include "elos/config/config.h"
#include "safu/common.h"
#include "safu/vector.h"

safuResultE_t elosAuthorizedProcessInitialize(safuVec_t *authorizedprocessfilters, samconfConfig_t const *const config);
safuResultE_t elosAuthorizedProcessDelete(safuVec_t *authorizedprocessfilters);
bool elosAuthorizedProcessCheck(safuVec_t *authorizedprocessfilters, elosProcessIdentity_t *process);
