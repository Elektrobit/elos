// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/builder_types.h"
#include "elos/rpnfilter/rpnfilter.h"

__BEGIN_DECLS

/*******************************************************************
 * Function:     elosRpnFilterBuilderNew
 *------------------------------------------------------------------
 * Description:  First step in the creation of a new RpnFilter.
 *               Allocates and initializes memory needed for
 *               the later creation steps.
 * Info:         The allocated elosRpnFilterBuilder_t
 *               must be freed with elosRpnFilterBuilderDelete().
 *               Used internally by elosRpnFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 * Input:        Filter parameters elosRpnFilterParam_t and a
 *               pointer to a elosRpnFilterBuilder_t.
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderNew(elosRpnFilterParam_t const *param, elosRpnFilterBuilder_t **data);

/*******************************************************************
 * Function:     elosRpnFilterBuilderTokenize
 *------------------------------------------------------------------
 * Description:  Converts a filter string into tokens that can be
 *               utilized by elosRpnFilterBuilderCompile().
 * Info:         Used internally by elosRpnFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderTokenize(elosRpnFilterBuilder_t *data);

elosRpnFilterResultE_t elosRpnFilterBuilderTokenizeParse(size_t *idx, elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosRpnFilterBuilderCompileToken
 *------------------------------------------------------------------
 * Description:  Converts a single token into a instruction.
 *
 * Info:         Passed to elosRpnFilterBuilderCompileWith(),
 *               shouldn't be called directly in normal use cases.
 * Input:        elosRpnFilterBuilderTokenEntry_t to be processed,
 *               elosRpnFilterBuilder_t for the result.
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderCompileToken(elosRpnFilterBuilderTokenEntry_t const *token,
                                                        elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosRpnFilterBuilderCompileWith
 *------------------------------------------------------------------
 * Description:  Converts tokens into fast to use instructions that
 *               are validated by elosRpnFilterBuilderFinalize()
 *               used later by elosRpnFilterExecute().
 * Info:         Used internally by elosRpnFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderCompile(elosRpnFilterBuilder_t *data);
elosRpnFilterResultE_t elosRpnFilterBuilderCompileWith(elosRpnFilterBuilder_t *data,
                                                       elosRpnFilterBuilderCompileFunc_t func[], size_t funcCount);

/*******************************************************************
 * Function:     elosRpnFilterBuilderFinalize
 *------------------------------------------------------------------
 * Description:  Checks the results from elosRpnFilterBuilderCompile()
 *               for errors and copies the results into a
 *               elosRpnFilter_t structure that can be used
 *               by elosRpnFilterExecute().
 * Info:         Used internally by elosRpnFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 * Input:        Pointers to existing elosRpnFilterBuilder_t
 *               and elosRpnFilter_t structures.
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderFinalize(elosRpnFilterBuilder_t *data, elosRpnFilter_t *filter);

/*******************************************************************
 * Function:     elosRpnFilterBuilderDelete
 *------------------------------------------------------------------
 * Description:  Frees the memory used during the creation stages.
 *
 * Info:         Used internally by elosRpnFilterCreate(),
 *               shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderDelete(elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosRpnFilterBuilderStackAppend
 *------------------------------------------------------------------
 * Description:  Helper function that appends a StackEntry
 *               to the given Builder struct.
 *               Shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilder_t
 *
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderStackAppend(elosRpnFilterStackEntry_t const *entry,
                                                       elosRpnFilterBuilder_t *data);

/*******************************************************************
 * Function:     elosRpnFilterBuilderStepAppend
 *------------------------------------------------------------------
 * Description:  Helper function that appends a StepEntry
 *               to the given Builder struct.
 *               Shouldn't be called directly in normal use cases.
 *
 * Input:        Pointer to existing elosRpnFilterBuilderStepEntry_t
 *               and elosRpnFilterBuilder_t
 *
 * Return:       - RPNFILTER_RESULT_OK on success,
 *               - RPNFILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterBuilderStepAppend(elosRpnFilterBuilderStepEntry_t const *step,
                                                      elosRpnFilterBuilder_t *data);

__END_DECLS
