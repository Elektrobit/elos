// SPDX-License-Identifier: MIT
#ifndef SAMCONF_SIGNATURE_H
#define SAMCONF_SIGNATURE_H

#include "samconf/samconf.h"

samconfConfigStatusE_t samconfSignatureLoaderGetSignatureFor(const char *file, char **signature, size_t *len);

#endif /* SAMCONF_SIGNATURE_H */
