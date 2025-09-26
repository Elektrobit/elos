// SPDX-License-Identifier: MIT
#pragma once

#include <safu/log.h>
#include <safu/result.h>
#include <sys/types.h>

#include "elos/event/event.h"
#include "elos/libelosdlt/dltmapper_types.h"
#include "elos/libelosdlt/types.h"

__BEGIN_DECLS
/*******************************************************************
 * Initialize dlt mapper with contextual values
 *
 * Parameters:
 *       mapper: a mapper whose members need to be initialized
 *       fileName: file name for dlt data, default is NULL
 *       appId: application id value, default is NULL
 *       hardwareid: hardware id value to be used, default retrieved from safuGetHardwareId()
 *       pid: process id to use, default is 0
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltMapperInit(elosDltMapper_t *mapper, char *fileName, char *appId, char *hardwareid, pid_t pid);

/*******************************************************************
 * Delete Members of dlt mapper members.
 *
 * Parameters:
 *       mapper: a mapper whose members need to be deleted
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltMapperDeleteMembers(elosDltMapper_t *mapper);

/*******************************************************************
 * Convert dlt data into an elos event. The event shall be null initialized.
 *
 * Parameters:
 *       mapper: a mapper that provides context to mapping dlt to elos event
 *       dltData: dlt data to be converted to elos event
 *       event: event into which dlt data is converted to
 *
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosDltMapperMapDataToEvent(elosDltMapper_t *mapper, elosDltData_t *dltData, elosEvent_t *event);

__END_DECLS
