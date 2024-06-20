// SPDX-License-Identifier: MIT
#ifndef ELOS_LOGLINE_MAPPER_DO_MAPPING_H
#define ELOS_LOGLINE_MAPPER_DO_MAPPING_H

#include <samconf/samconf.h>

#include "logline_mapper_utest.h"

int elosLoglineMapperInitUtestCleanUp(void **state);
int elosLoglineMapperInitUtestInit(void **state, const char *config);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperInitSuccess)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperInitSectionedMessageCodes)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperInitErrDeepNestedMessageCodes)
TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperInitNullConfig)
#endif
