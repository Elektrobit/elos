// SPDX-License-Identifier: MIT
#pragma once

#include <pthread.h>

#include "elos/plugincontrol/types.h"

__BEGIN_DECLS

void *elosPluginControlWorkerThread(void *data);

__END_DECLS
