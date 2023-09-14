// SPDX-License-Identifier: MIT
#pragma once

#include "elos/processfilter/processfilter.h"
#include "elos/rpnfilter/vector.h"

typedef elosRpnFilterVector_t elosProcessFilterVector_t;

#define elosProcessFilterVectorInit(__v, __c)         elosRpnFilterVectorInit(__v, __c)
#define elosProcessFilterVectorPush(__v, __e)         elosRpnFilterVectorPush(__v, __e)
#define elosProcessFilterVectorIterate(__v, __f, __d) elosRpnFilterVectorIterate(__v, __f, __d)
#define elosProcessFilterVectorDeleteMembers(__v)     elosRpnFilterVectorDeleteMembers(__v)
