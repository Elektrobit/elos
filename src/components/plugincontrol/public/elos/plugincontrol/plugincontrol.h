// SPDX-License-Identifier: MIT
#pragma once

#include "elos/plugincontrol/defines.h"
#include "elos/plugincontrol/types.h"

__BEGIN_DECLS

/*******************************************************************
 * Initializes an existing PluginControl data structure
 *
 * Parameters:
 *      control : Pointer to the PluginControl data structure
 *      param   : Parameters for initialization
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlInitialize(elosPluginControl_t *control, elosPluginControlParam_t const *param);

/*******************************************************************
 * Allocates and initializes a PluginControl data structure
 *
 * Parameters:
 *      control : Pointer to a pointer that will contain
 *                the new PluginControl data structure
 *      param   : Parameters for initialization
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlNew(elosPluginControl_t **control, elosPluginControlParam_t const *param);

/*******************************************************************
 * Loads the Plugin shared object specified during initialization
 * and checks if everything needed is present, as well as the
 * type of the Plugin itself (Scanner, Storage, ...).
 * Also starts a worker thread and calls the Plugins 'load' function,
 * the worker thread will sleep until elosPluginControlStart is called.
 * The function call will return once the worker thread is ready,
 * so no further synchronization is needed.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already loaded
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlLoad(elosPluginControl_t *control);

/*******************************************************************
 * Unloads the Plugin by calling its 'unload' function,
 * closing the worker thread and freeing all memory associated with it afterwards.
 * Will call elosPluginControlStop first if the Plugin is still running.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already unloaded
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlUnload(elosPluginControl_t *control);

/*******************************************************************
 * Starts the Plugin by activating the Plugins worker thread
 * and calling the Plugins 'start' function. Returns when the Plugin
 * is running, so no further synchronization is needed.
 * The Plugin needs to be loaded for this command to work.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already started
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlStart(elosPluginControl_t *control);

/*******************************************************************
 * Stops the Plugin via the Plugins 'stop' function.
 * The Plugin needs to be loaded for this command to work.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already stopped
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlStop(elosPluginControl_t *control);

/*******************************************************************
 * Frees all member data of an PluginControl data structure.
 * Will call elosPluginControlUnload first if the Plugin is still loaded.
 *
 * Parameters:
 *      control : Pointer to a PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already deleted
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlDeleteMembers(elosPluginControl_t *control);

/*******************************************************************
 * Frees all member data of an PluginControl data structure by calling
 * elosPluginControlDeleteMembers, then frees the data structure itself.
 *
 * Parameters:
 *      control : Pointer to a pointer of an PluginControl data structure
 * Returns:
 *      - `SAFU_RESULT_OK` on success or when Plugin is already deleted
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlDelete(elosPluginControl_t **control);

/*******************************************************************
 * Get the Plugins name based on the configuration associated with it.
 * The returned name string will be still associated with the samconf
 * data structure and must not be freed to avoid problems.
 *
 * Parameters:
 *      control : Pointer to of an PluginControl data structure
 *      name    : Pointer to a pointer to the string containing the name
 * Returns:
 *      - `SAFU_RESULT_OK` on success
 *      - `SAFU_RESULT_FAILED` on failure
 ******************************************************************/
safuResultE_t elosPluginControlGetName(const elosPluginControl_t *control, const char **name);

__END_DECLS
