// SPDX-License-Identifier: MIT
#pragma once

#include "elos/eventfilter/eventfilter_types.h"

/*******************************************************************
 * Function:     elosEventFilterInit
 *------------------------------------------------------------------
 * Description:  Initializes an existing elosRpnFilter
 *
 * Input:        Pointer to the memory area that will be
 *               initialized with default values after compiling
 *
 * Info:         Needs to be freed with elosEventFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterInit(elosEventFilter_t *filter);

/*******************************************************************
 * Function:     elosEventFilterNew
 *------------------------------------------------------------------
 * Description:  Allocates an elosRpnFilter
 *
 * Input:        Pointer to a pointer that will contain the newly
 *               allocated elosRpnFilter after calling the function.
 *
 * Info:         The resulting elosRpnFilter is uninitialized
 *               Needs to be freed with elosRpnFilterDelete()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterNew(elosEventFilter_t **filter);

/*******************************************************************
 * Function:     elosEventFilterDeepCopy
 *------------------------------------------------------------------
 * Description:  Makes a deep copy of elosRpnFilter. The copy is
 *               completely independent of the original
 *               and needs to be freed separately.
 *
 * Input:        Two elosRpnFilter pointers (from and to)
 * Info:         'to' needs to be freed with elosEventFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterDeepCopy(elosEventFilter_t *to, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosEventFilterClone
 *------------------------------------------------------------------
 * Description:  Duplicates elosRpnFilter into a newly allocated
 *               memory region. The copy is completely independent
 *               of the original and needs to be freed separately.
 *
 * Input:        Two elosRpnFilter pointers (from and to)
 * Info:         'to' needs to be freed with elosEventFilterDelete()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterClone(elosEventFilter_t **filter, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosEventFilterDeleteMembers
 *------------------------------------------------------------------
 * Description:  Frees memory used by elosRpnFilter members
 * Input:        Pointer to the elosRpnFilter to free
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterDeleteMembers(elosEventFilter_t *filter);

/*******************************************************************
 * Function:     elosEventFilterDeleteMembers
 *------------------------------------------------------------------
 * Description:  Frees memory used by elosRpnFilter members
 *
 * Input:        Pointer to the memory area that will be
 *               initialized with default values after compiling
 *
 * Info:         Needs to be freed with elosEventFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterDelete(elosEventFilter_t *filter);

/*******************************************************************
 * Function:     elosEventFilterCreate
 *------------------------------------------------------------------
 * Description:  Create new filter to be used with elosEventFilterExecute
 * Input:        Initialized elosRpnFilter and parameters
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterCreate(elosEventFilter_t *filter, const elosEventFilterParam_t *param);

/*******************************************************************
 * Function:     elosEventFilterExecute
 *------------------------------------------------------------------
 * Description:  Runs an elosRpnFilter on the given data
 * Input:        elosRpnFilter and parameters
 *
 * Return:       - FILTER_RESULT_MATCH on success with a match,
 *               - FILTER_RESULT_NO_MATCH on success with no match,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosEventFilterExecute(elosEventFilter_t const *filter, elosEventFilterStack_t const *param,
                                              elosEvent_t *input);
