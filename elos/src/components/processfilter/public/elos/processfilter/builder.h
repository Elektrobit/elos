// SPDX-License-Identifier: MIT
#pragma once

#include "elos/processfilter/processfilter.h"
#include "elos/processfilter/step_types.h"
#include "elos/rpnfilter/builder.h"

#define elosProcessFilterBuilderNew    elosRpnFilterBuilderNew
#define elosProcessFilterBuilderDelete elosRpnFilterBuilderDelete

typedef enum elosProcessFilterBuilderTokenTypeE {
    PROCESSFILTER_TOKEN_PROCESS = RPNFILTER_TOKEN_TYPE_COUNT,
    PROCESSFILTER_TOKEN_TYPE_COUNT,
} elosProcessFilterBuilderTokenTypeE_t;

elosRpnFilterResultE_t elosProcessFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                            elosRpnFilterBuilder_t *data);

elosRpnFilterResultE_t elosProcessFilterBuilderTokenize(elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosProcessFilterBuilderCompile
 *------------------------------------------------------------------
 * Description:  Converts tokens into fast to use instructions that
 *               are validated by elosProcessFilterBuilderFinalize()
 *               used later by elosProcessFilterExecute().
 * Info:         Used internally by elosProcessFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterBuilderCompile(elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosProcessFilterBuilderFinalize
 *------------------------------------------------------------------
 * Description:  Checks the results from elosProcessFilterBuilderCompile()
 *               for errors and copies the results into a
 *               elosRpnFilter_t structure that can be used
 *               by elosProcessFilterExecute().
 * Info:         Used internally by elosProcessFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointers to existing elosRpnFilterBuilder_t
 *               and elosRpnFilter_t structures.
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterBuilderFinalize(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter);
