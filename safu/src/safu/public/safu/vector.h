// SPDX-License-Identifier: MIT
#pragma once

#include <safu/vector_types.h>

/*******************************************************************
 * Initialize a new vector with a given count and element size.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to store the vector struct.
 *      elementCount (uint32_t): Number of elements that the vector should store.
 *      elementSize (uint32_t): Size of the elements type.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case
 ******************************************************************/
int safuVecCreate(safuVec_t *vec, uint32_t elements, uint32_t elementSize);

/*******************************************************************
 * Free the memory used by a safu Vector and resets all its members to zero.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector to be free'd.
 * Returns:
 *      - `0` if successful
 *      - `-1` if vec is `NULL`
 ******************************************************************/
int safuVecFree(safuVec_t *vec);

/*******************************************************************
 * Add an element to an existing safu vector.
 * If the vector is already full, the size of the vector will be increased by one
 * element. If the specified position is not the last one, the existing entries
 * will be shifted one to the right, starting from the specified position.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector which should gain a new element.
 *      idx (uint32_t): Index in the vector to which the element should be added.
 *      element (const void *): Pointer to the element that should be added to the vector.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case and leaves vec unchanged
 ******************************************************************/
int safuVecInsert(safuVec_t *vec, uint32_t idx, const void *element);

/*******************************************************************
 * Remove the element at an specified index from a safu vector.
 * It moves all consecutive elements after the selected element by one position
 * and then zeros the last element.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector which should have an element removed.
 *      idx (uint32_t): Index in the vector where the element should be removed.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case and leaves vec unchanged
 ******************************************************************/
int safuVecRemove(safuVec_t *vec, uint32_t idx);

/*******************************************************************
 * Add a new element to the end of a safu vector.
 * This is a shortcut for `safuVecInsert(vec, vec->elementCount, element)`
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector which should gain a new element.
 *      element (const void *): Pointer to the element that should be added to
 *              the vector.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case and leaves vec unchanged
 ******************************************************************/
int safuVecPush(safuVec_t *vec, const void *element);

/*******************************************************************
 * Remove the last element of a safu vector.
 * This function does not free the element in question.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector which should have its last
 *              element removed.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case and leaves vec unchanged
 ******************************************************************/
int safuVecPop(safuVec_t *vec);

/*******************************************************************
 * Overwrite the element at a specified position in the safu vector
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector which should be modified.
 *      idx (uint32_t): Index in the vector where the element should overwritten.
 *      element (const void *): Pointer to the element that should be inserted.
 * Returns:
 *      - `0` if successful
 *      - `-1` in error case and leaves vec unchanged
 ******************************************************************/
int safuVecSet(safuVec_t *vec, uint32_t idx, const void *element);

/*******************************************************************
 * Retrieve the pointer to an element inside a safu vector
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector from which to retrieve the element.
 *      idx (uint32_t): Index in the vector where the element to be retrieved is.
 *              stored
 * Returns:
 *      - pointer to the element in the vector
 *      - `NULL` in an error case
 ******************************************************************/
void *safuVecGet(const safuVec_t *vec, uint32_t idx);

/*******************************************************************
 * Retrieve the pointer to the last element inside  safu vector.
 * This mainly a wrapper for `safuVecGet(vec, (vec->elementCount - 1))`.
 *
 * Parameters:
 *      vec (safuVec_t *): Pointer to the vector from which to retrieve the element.
 * Returns:
 *      - pointer to the element in the vector
 *      - `NULL` in an error case
 ******************************************************************/
void *safuVecGetLast(const safuVec_t *vec);

/*******************************************************************
 * Get the number of elements the vector currently holds.
 *
 * Parameters:
 *      vec (const safuVec_t *): Pointer to the vector of which the size is asked.
 * Returns:
 *      - `vec->elementCount`
 *      - `0` if vector is invalid
 ******************************************************************/
uint32_t safuVecElements(const safuVec_t *vec);

/*******************************************************************
 * Find the index of an element with specified properties inside a safu vector
 * using a match-function.
 *
 * Parameters:
 *      vec (const safuVec_t *): Pointer to the vector to be searched.
 *      startIdx (uint32_t): The position in the vector to start the search at.
 *      matchFunc (safuVecFunc_t): A helper function to compare an element in the
 *              vector with the specified element that serves as the search base.
 *      matchData (const void *): The element that serves as the search base.
 *      resultIdx (uint32_t *): buffer to return the index of the found element
 * Returns:
 *      - `1` if found
 *      - `0` if not found
 *      - `-1` in error case.
 ******************************************************************/
int safuVecFind(const safuVec_t *vec, uint32_t startIdx, safuVecFunc_t matchFunc, const void *matchData,
                uint32_t *resultIdx);

/*******************************************************************
 * Find an element inside a safu vector that meets specified requirements and
 * retrieve the matching element.
 * This function is a wrapper for the combination of `safuVecFind` and `safuVecGet`.
 *
 * Parameters:
 *      vec (const safuVec_t *): Pointer to the vector to be searched.
 *      element (void **): Buffer to return the found element from the vector.
 *      idx (uint32_t *): Buffer to store the index of the found element.
 *              If this value is not NULL, it also serves as the starting index
 *              for the search.
 *      matchFunc (safuVecFunc_t): A helper function to compare an element in the
 *              vector with the specified element that serves as the search base.
 *      matchData (const void *): The element that serves as the search base.
 * Returns:
 *      - `1` if found
 *      - `0` if not found
 *      - `-1` in error case
 ******************************************************************/
int safuVecFindGet(const safuVec_t *vec, void **element, uint32_t *idx, safuVecFunc_t matchFunc, const void *matchData);

/*******************************************************************
 * Find an element inside a safu vector that meets specified requirements and
 * delete it.
 * This function is a wrapper for the combination of `safuVecFind` and `safuVecRemove`.
 *
 * Parameters:
 *      vec (const safuVec_t *): Pointer to the vector to be searched.
 *      matchFunc (safuVecFunc_t): A helper function to compare an element in the
 *              vector with the specified element that serves as the search base.
 *      matchData (const void *): The element that serves as the search base.
 * Returns:
 *      - `1` if found
 *      - `0` if not found
 *      - `-1` in error case.
 ******************************************************************/
int safuVecFindRemove(safuVec_t *vec, safuVecFunc_t matchFunc, const void *matchData);

/*******************************************************************
 * Call a safuVecFunc on every element of the vector.
 *
 * Parameters:
 *      vec (const safuVec_t *): Pointer to the vector to be iterated.
 *      func (safuVecFunc_t): Function to be executed for every member of
 *              the vector
 *      data (const void *): Additional data to be provided to the iterated function
 * Returns:
 *      - `1` if found
 *      - `0` if not found, which leaves the vec unchanged
 *      - `-1` in error case and leaves the vec unchanged
 ******************************************************************/
#define safuVecIterate(__v, __f, __d) safuVecFind(__v, 0, __f, __d, NULL)
