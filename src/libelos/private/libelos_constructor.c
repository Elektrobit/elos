// SPDX-License-Identifier: MIT

#include "libelos_constructor.h"

#include <stdlib.h>

#include "safu/common.h"

bool elosLoggingEnabled = false;
__attribute__((constructor)) void elosLibraryConstructor(void) {
    const char *libElosLogEnv = safuGetEnvOr("LIBELOS_LOG", NULL);
    if (libElosLogEnv != NULL) {
        elosLoggingEnabled = true;
    }
}
