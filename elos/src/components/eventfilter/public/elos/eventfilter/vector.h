// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/vector.h"

#define elosEventFilterVectorInit(__v, __c)         elosRpnFilterVectorInit(__v, __c)
#define elosEventFilterVectorPush(__v, __e)         elosRpnFilterVectorPush(__v, __e)
#define elosEventFilterVectorIterate(__v, __f, __d) elosRpnFilterVectorIterate(__v, __f, __d)
#define elosEventFilterVectorDeleteMembers(__v)     elosRpnFilterVectorDeleteMembers(__v)
