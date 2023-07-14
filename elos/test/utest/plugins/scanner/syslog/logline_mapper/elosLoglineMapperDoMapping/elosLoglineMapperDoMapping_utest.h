// SPDX-License-Identifier: MIT
#ifndef ELOS_LOGLINE_MAPPER_DO_MAPPING_H
#define ELOS_LOGLINE_MAPPER_DO_MAPPING_H

#include "logline_mapper_utest.h"

typedef struct elosTestState {
    char *timezone;
    samconfConfig_t *childrenData;
    elosLoglineMapper_t mapper;
} elosTestState_t;

int elosLoglineMapperDoMappingUtestInit(void **state);
int elosLoglineMapperDoMappingUtestCleanUp(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperDoMappingSuccess)
#endif
