// SPDX-License-Identifier: MIT
#pragma once

#include <safu/common.h>

#include "elos/eventprocessor/types.h"

safuResultE_t elosIdManagerInitialize(elosIdManager_t *im, elosIdManagerParam_t const *param);
safuResultE_t elosIdManagerDeleteMemebers(elosIdManager_t *im);
safuResultE_t elosIdManagerGenerate(elosIdManager_t *im, elosId_t *id);
safuResultE_t elosIdManagerDestroy(elosIdManager_t *im, elosId_t id);
