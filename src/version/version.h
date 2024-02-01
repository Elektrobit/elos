// SPDX-License-Identifier: MIT
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/cdefs.h>

typedef struct elosVersion {
    uint8_t major;
    uint8_t minor;
    uint8_t micro;
    const char *git;
} elosVersion_t;

extern const elosVersion_t elosVersion;

__BEGIN_DECLS

const char *elosGetVersionString(void);
void elosPrintVersion(FILE *const destination, const char *prefix) __attribute__((nonnull(1)));
bool elosIsVersionRequested(const char *const *const argv, int argc) __attribute__((nonnull(1)));

__END_DECLS
