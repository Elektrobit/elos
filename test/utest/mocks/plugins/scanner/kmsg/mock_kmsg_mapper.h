// SPDX-License-Identifier: MIT
#ifndef ELOS_KMSG_MAPPER_MOCK_H
#define ELOS_KMSG_MAPPER_MOCK_H

#include <cmocka_extensions/mock_extensions.h>
#ifdef MOCK_KMSG_FUNC_LINKER_WRAPPING
#include <cmocka_extensions/mock_func_wrap.h>
#else
#include <cmocka_extensions/mock_func_weak.h>
#endif

#include <kmsg_mapper.h>

MOCK_FUNC_PROTOTYPE(elosKmsgMapperInit, safuResultE_t, elosKmsgMapper_t *mapper)

#endif /* ELOS_KMSG_MAPPER_MOCK_H */