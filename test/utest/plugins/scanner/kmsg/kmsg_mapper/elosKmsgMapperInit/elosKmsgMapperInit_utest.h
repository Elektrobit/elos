// SPDX-License-Identifier: MIT
#ifndef ELOS_KMSG_MAPPER_INIT_UTEST_H
#define ELOS_KMSG_MAPPER_INIT_UTEST_H

#include <cmocka_extensions/cmocka_extensions.h>
#include <kmsg_mapper.h>
#include <safu/common.h>

#include "cmocka_mocks/mock_libc.h"

TEST_CASE_FUNC_PROTOTYPES(elosTestElosKmsgMapperInitSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosKmsgMapperInitErrOpenProc)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosKmsgMapperInitErrFscanfFail)

#endif /* ELOS_KMSG_MAPPER_INIT_UTEST_H */
