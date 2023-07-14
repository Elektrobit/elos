// SPDX-License-Identifier: MIT
#ifndef ELOS_EVENTFILTER_BUILDER_H
#define ELOS_EVENTFILTER_BUILDER_H

#include "elos/eventfilter/eventfilter.h"
#include "elos/eventfilter/step_types.h"
#include "elos/rpnfilter/builder.h"

#define elosEventFilterBuilderNew    elosRpnFilterBuilderNew
#define elosEventFilterBuilderDelete elosRpnFilterBuilderDelete

typedef enum elosEventFilterBuilderTokenTypeE {
    EVENTFILTER_TOKEN_EVENT = RPNFILTER_TOKEN_TYPE_COUNT,
    EVENTFILTER_TOKEN_TYPE_COUNT,
} elosEventFilterBuilderTokenTypeE_t;

elosRpnFilterResultE_t elosEventFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                          elosRpnFilterBuilder_t *data);

elosRpnFilterResultE_t elosEventFilterBuilderTokenize(elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosEventFilterBuilderCompile
 *------------------------------------------------------------------
 * Description:  Converts tokens into fast to use instructions that
 *               are validated by elosEventFilterBuilderFinalize()
 *               used later by elosEventFilterExecute().
 * Info:         Used internally by elosEventFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterBuilderCompile(elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosEventFilterBuilderFinalize
 *------------------------------------------------------------------
 * Description:  Checks the results from elosEventFilterBuilderCompile()
 *               for errors and copies the results into a
 *               elosRpnFilter_t structure that can be used
 *               by elosEventFilterExecute().
 * Info:         Used internally by elosEventFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 * Input:        Pointers to existing elosRpnFilterBuilder_t
 *               and elosRpnFilter_t structures.
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterBuilderFinalize(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter);

#endif /* ELOS_EVENTFILTER_BUILDER_H */
