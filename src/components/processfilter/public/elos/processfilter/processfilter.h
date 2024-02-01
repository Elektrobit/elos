// SPDX-License-Identifier: MIT
#pragma once

#include "elos/common/process.h"
#include "elos/processfilter/processfilter_types.h"

__BEGIN_DECLS

/*******************************************************************
 * Function:     elosProcessFilterInit
 *------------------------------------------------------------------
 * Description:  Initializes an existing elosRpnFilter
 *
 * Input:        Pointer to the memory area that will be
 *               initialized with default values after compiling
 *
 * Info:         Needs to be freed with elosProcessFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterInit(elosProcessFilter_t *filter);

/*******************************************************************
 * Function:     elosProcessFilterNew
 *------------------------------------------------------------------
 * Description:  Allocates an elosRpnFilter
 *
 * Input:        Pointer to a pointer that will contain the newly
 *               allocated elosRpnFilter after calling the function.
 *
 * Info:         The resulting elosRpnFilter is uninitialized
 *               Needs to be freed with elosRpnFilterDelete()
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterNew(elosProcessFilter_t **filter);

/*******************************************************************
 * Function:     elosProcessFilterDeepCopy
 *------------------------------------------------------------------
 * Description:  Makes a deep copy of elosRpnFilter. The copy is
 *               completely independent of the original
 *               and needs to be freed separately.
 *
 * Input:        Two elosRpnFilter pointers (from and to)
 * Info:         'to' needs to be freed with elosProcessFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterDeepCopy(elosProcessFilter_t *to, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosProcessFilterClone
 *------------------------------------------------------------------
 * Description:  Duplicates elosRpnFilter into a newly allocated
 *               memory region. The copy is completely independent
 *               of the original and needs to be freed separately.
 *
 * Input:        Two elosRpnFilter pointers (from and to)
 * Info:         'to' needs to be freed with elosProcessFilterDelete()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterClone(elosProcessFilter_t **filter, const elosRpnFilter_t *from);

/*******************************************************************
 * Function:     elosProcessFilterDeleteMembers
 *------------------------------------------------------------------
 * Description:  Frees memory used by elosRpnFilter members
 * Input:        Pointer to the elosRpnFilter to free
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterDeleteMembers(elosProcessFilter_t *filter);

/*******************************************************************
 * Function:     elosProcessFilterDeleteMembers
 *------------------------------------------------------------------
 * Description:  Frees memory used by elosRpnFilter members
 *
 * Input:        Pointer to the memory area that will be
 *               initialized with default values after compiling
 *
 * Info:         Needs to be freed with elosProcessFilterDeleteMembers()
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterDelete(elosProcessFilter_t *filter);

/*******************************************************************
 * Function:     elosProcessFilterCreate
 *------------------------------------------------------------------
 * Description:  Create new filter to be used with elosProcessFilterExecute
 * Input:        Initialized elosRpnFilter and parameters
 *
 * Return:       - FILTER_RESULT_OK on success,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterCreate(elosProcessFilter_t *filter, const elosProcessFilterParam_t *param);

/*******************************************************************
 * Function:     elosProcessFilterExecute
 *------------------------------------------------------------------
 * Description:  Runs an elosRpnFilter on the given data
 * Input:        elosRpnFilter and parameters
 *
 * Return:       - FILTER_RESULT_MATCH on success with a match,
 *               - FILTER_RESULT_NO_MATCH on success with no match,
 *               - FILTER_RESULT_ERROR on failure.
 ******************************************************************/
elosRpnFilterResultE_t elosProcessFilterExecute(elosProcessFilter_t const *filter,
                                                elosProcessFilterStack_t const *param, elosProcessIdentity_t *input);

__END_DECLS
