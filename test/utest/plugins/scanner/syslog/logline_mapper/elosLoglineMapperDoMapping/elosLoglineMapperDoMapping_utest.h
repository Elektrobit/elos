// SPDX-License-Identifier: MIT
#ifndef ELOS_LOGLINE_MAPPER_DO_MAPPING_H
#define ELOS_LOGLINE_MAPPER_DO_MAPPING_H

#include "logline_mapper_utest.h"

typedef struct elosTestState {
    char *timezone;
    samconfConfig_t config;
    elosLoglineMapper_t mapper;
} elosTestState_t;

int elosLoglineMapperDoMappingUtestInit(void **state, const char *config);
int elosLoglineMapperDoMappingUtestCleanUp(void **state);

TEST_CASE_FUNC_PROTOTYPES(elosTestElosLoglineMapperDoMappingSuccess)
#endif
