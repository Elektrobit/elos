// SPDX-License-Identifier: MIT
#pragma once

#include "elos/rpnfilter/rpnfilter_types.h"

__BEGIN_DECLS

/*******************************************************************
 * Function:     elosRpnFilterInit
 *------------------------------------------------------------------
 * Description:
 *     Initializes an existing elosRpnFilter
 * Input:
 *     Pointer to the memory area that will be
 *     initialized with default values after compiling
 * Info:
 *     Needs to be freed with elosRpnFilterDeleteMembers()
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterInit(elosRpnFilter_t *filter);

/*******************************************************************
 * Function:     elosRpnFilterNew
 *------------------------------------------------------------------
 * Description:
 *     Allocates an elosRpnFilter
 * Input:
 *     Pointer to a pointer that will contain the newly
 *     allocated elosRpnFilter after calling the function.
 * Info:
 *     The resulting elosRpnFilter is uninitialized
 *     Needs to be freed with elosRpnFilterDelete()
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterNew(elosRpnFilter_t **filter);

/*******************************************************************
 * Function:     elosRpnFilterDeepCopy
 *------------------------------------------------------------------
 * Description:
 *     Makes a deep copy of elosRpnFilter. The copy is
 *     completely independent of the original
 *     and needs to be freed separately.
 * Input:
 *     Two elosRpnFilter pointers (from and to)
 * Info:
 *     'to' needs to be freed with elosRpnFilterDeleteMembers()
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterDeepCopy(elosRpnFilter_t *to, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosRpnFilterClone
 *------------------------------------------------------------------
 * Description:
 *     Duplicates elosRpnFilter into a newly allocated
 *     memory region. The copy is completely independent
 *     of the original and needs to be freed separately.
 *
 * Input:
 *     Two elosRpnFilter pointers (from and to)
 * Info:
 *     'to' needs to be freed with elosRpnFilterDelete()
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterClone(elosRpnFilter_t **filter, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosRpnFilterDeleteMembers
 *------------------------------------------------------------------
 * Description:
 *     Frees memory used by elosRpnFilter members
 * Input:
 *     Pointer to the elosRpnFilter to free
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterDeleteMembers(elosRpnFilter_t *filter);

/*******************************************************************
 * Function:     elosRpnFilterDelete
 *------------------------------------------------------------------
 * Description:
 *     Frees memory used by elosRpnFilter members
 * Input:
 *     Pointer to the memory area that will be
 *     initialized with default values after compiling
 * Info:
 *     Needs to be freed with elosRpnFilterDeleteMembers()
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterDelete(elosRpnFilter_t *filter);

/*******************************************************************
 * Function:     elosRpnFilterCreate
 *------------------------------------------------------------------
 * Description:
 *     Create new filter to be used with elosRpnFilterExecute
 * Info:
 *     Combines elosRpnFilterBuilderNew
 *     elosRpnFilterBuilderTokenize, elosRpnFilterBuilderCompile
 *     and elosRpnFilterBuilderFinalize into an easy to use function.
 * Input:
 *     Initialized elosRpnFilter and parameters
 * Return:
 *     - FILTER_RESULT_OK on success,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterCreate(elosRpnFilter_t *filter, const elosRpnFilterParam_t *param);

/*******************************************************************
 * Function:     elosRpnFilterExecute
 *------------------------------------------------------------------
 * Description:
 *     Runs an elosRpnFilter on the given data
 * Input:
 *     elosRpnFilter and parameters
 * Return:
 *     - FILTER_RESULT_MATCH on success with a match,
 *     - FILTER_RESULT_NO_MATCH on success with no match,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterExecute(const elosRpnFilter_t *filter, const elosRpnFilterStack_t *input);

/*******************************************************************
 * Function:     elosRpnFilterExecuteResult
 *------------------------------------------------------------------
 * Description:
 *     Runs an elosRpnFilter on the given data and
 *     returns the remaining values on the filter stack.
 *     Useful for more complex operations.
 *
 * Input:
 *     Initialized elosRpnFilter and parameters
 * Return:
 *     - FILTER_RESULT_MATCH on success with a match,
 *     - FILTER_RESULT_NO_MATCH on success with no match,
 *     - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosRpnFilterExecuteResult(const elosRpnFilter_t *filter, const elosRpnFilterStack_t *input,
                                                  elosRpnFilterStack_t *output);

__END_DECLS
