// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/plugincontrol.h"
#include "safu/vector.h"

__BEGIN_DECLS

/*******************************************************************
 * Initialized a PluginControl vector data structure.
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 *      elements      : Number of elements the Vector can hold without resizing
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorInitialize(elosPluginControlVector_t *controlVector, size_t elements);

/*******************************************************************
 * Pushes a new element to a PluginControl vector data structure.
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 *      control       : PluginControl data structure to be added
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorPush(elosPluginControlVector_t *controlVector, elosPluginControl_t *control);

/*******************************************************************
 * Iterates through all elements of an PluginControl vector
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 *      func          : Function to be called for each element
 *      data          : Data to be used with the function (e.g. for comparison)
 *      iterResult    : Return value of the iteration
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorIterate(elosPluginControlVector_t *controlVector, safuVecFunc_t *func, void *data,
                                             int *iterResult);

/*******************************************************************
 * Search a PluginControl vector for a PluginControl with the given id
 * and return it when found.
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 *      id            : id of the PluginControl
 *      control       : Pointer to a pointer that will contain the PluginControl when found
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorGetById(elosPluginControlVector_t *controlVector, elosPluginId_t id,
                                             elosPluginControl_t **control);

/*******************************************************************
 * Remove a PluginControl from a PluginControl vector with the given id.
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 *      id            : id of the PluginControl
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorRemoveById(elosPluginControlVector_t *controlVector, elosPluginId_t id);

/*******************************************************************
 * Frees all member data from a PluginControl data structure
 *
 * Parameters:
 *      controlVector : Pointer to of an PluginControlVector data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlVectorDeleteMembers(elosPluginControlVector_t *controlVector);

__END_DECLS
