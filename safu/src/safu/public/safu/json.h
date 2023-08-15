// SPDX-License-Identifier: MIT
#pragma once

#include <json-c/json_types.h>
#include <stddef.h>
#include <stdint.h>
#include <time.h>

/*******************************************************************
 * Add an empty json array value to the given json object. This function is mostly
 * a wrapper for json-c's json_object_new_array
 *
 * Parameters:
 *      jobj (json_object *): Json Object to which an array should be added.
 *      name (const char *): Key to store the new array under.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to json array
 ******************************************************************/
struct json_object *safuJsonAddNewArray(struct json_object *jobj, const char *name);

/*******************************************************************
 * Add an json array value with a given number of pre-allocated slots to the given
 * json object. This function is mostly a wrapper for json-c's
 * json_object_new_array_ext.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which an array should be added.
 *      name (const char *): Key to store the new array under.
 *      initialSize (int): Number of array slots to allocate at creation.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to json array
 ******************************************************************/
struct json_object *safuJsonAddNewArrayOfInitialSize(struct json_object *jobj, const char *name, int initialSize);

/*******************************************************************
 * Add an integer value to a given json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the integer should be added.
 *      name (const char *): Key to store the new integer under.
 *      val (int32_t): Value to store in the object.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to json integer
 ******************************************************************/
struct json_object *safuJsonAddNewInt(struct json_object *jobj, const char *name, int32_t val);

/*******************************************************************
 * Add an long integer value to a given json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the int64 should be added.
 *      name (const char *): Key to store the new integer under.
 *      val (int64_t): Value to store in the object.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to json int64
 ******************************************************************/
struct json_object *safuJsonAddNewInt64(struct json_object *jobj, const char *name, int64_t val);

/*******************************************************************
 * Add an new empty json object value to a given existing json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the new object should be added.
 *      name (const char *): Key to store the new object under.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to new json object
 ******************************************************************/
struct json_object *safuJsonAddNewObject(struct json_object *jobj, const char *name);

/*******************************************************************
 * Add an json string value to a given json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the string should be added.
 *      name (const char *): Key to store the new string under.
 *      val (const char *): String to be stored in json object.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to new json object
 ******************************************************************/
struct json_object *safuJsonAddNewString(struct json_object *jobj, const char *name, const char *val);

/*******************************************************************
 * Add an long unsinged integer value to a given json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the uint64 should be added.
 *      name (const char *): Key to store the new integer under.
 *      val (uint64_t): Value to store in the object.
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to json uint64
 ******************************************************************/
struct json_object *safuJsonAddNewUint64(struct json_object *jobj, const char *name, uint64_t val);

/*******************************************************************
 * Add an existing json object as a value to another json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object to which the the other json object should be added.
 *      name (const char *): Key to store the json object under.
 *      jdata (uint64_t): Json object to store.
 * Returns:
 *      - `>= 0` in success case
 *      - `< 0` in error case
 ******************************************************************/
int safuJsonAddObject(struct json_object *jobj, const char *name, struct json_object *jdata);

/*******************************************************************
 * Get the value of a 32-bit integer stored in a json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the integer.
 *      name (const char *): Key the int32 is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the int32, if the given json object
 *              is an array
 *      val (int32_t *): Pointer to store the value in.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetInt32(const struct json_object *jobj, const char *name, size_t idx, int32_t *val);

/*******************************************************************
 * Get the value of a 64-bit integer stored in a json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the integer.
 *      name (const char *): Key the int64 is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the int64, if the given json object
 *              is an array
 *      val (int64_t *): Pointer to store the int64 in.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetInt64(const struct json_object *jobj, const char *name, size_t idx, int64_t *val);

/*******************************************************************
 * Get a pointer to a json object stored inside another json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the json object.
 *      name (const char *): Key the json object is stored under,
 *              if the given json object is an object(map)
 *      val (size_t): Index of the json object, if the given json object
 *              is an array
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to the requested json object.
 ******************************************************************/
struct json_object *safuJsonGetObject(const struct json_object *jobj, const char *name, size_t idx);

/*******************************************************************
 * Get a pointer to a json array stored inside an json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the json object.
 *      name (const char *): Key the json array is stored under,
 *              if the given json object is an object(map)
 *      val (size_t): Index of the json array, if the given json object
 *              is an array
 *      len (size_t *): Buffer to store the size of the stored json array
 * Returns:
 *      - `NULL` in error case
 *      - Pointer to the requested json array.
 ******************************************************************/
struct json_object *safuJsonGetArray(const struct json_object *jobj, const char *name, size_t idx, size_t *len);

/*******************************************************************
 * Get the value of an unsigned 32-bit integer stored in a json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the uint.
 *      name (const char *): Key the uint is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the uint, if the given json object
 *              is an array
 *      val (uint32_t *): Pointer to store the value in.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetUint32(const struct json_object *jobj, const char *name, size_t idx, uint32_t *val);

/*******************************************************************
 * Get the value of an unsigned 64-bit integer stored in a json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the uint64.
 *      name (const char *): Key the uint64 is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the uint64, if the given json object
 *              is an array
 *      val (uint64_t *): Pointer to store the uint64 in.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetUint64(const struct json_object *jobj, const char *name, size_t idx, uint64_t *val);

/*******************************************************************
 * Get the value of an string stored in a json object.
 *
 * Parameters:
 *      jobj (json_object *): Json object from which to read the uint.
 *      name (const char *): Key the string is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the string, if the given json object
 *              is an array
 *      val (const char **): Pointer to store the value in.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetString(const struct json_object *jobj, const char *name, size_t idx, const char **val);

/*******************************************************************
 * Add Timestamp to an json object. A Timestamp is a json array containing two
 * json int64 numbers, which encode the seconds and nanoseconds of a unix time
 * stamp.
 *
 * Parameters:
 *      jobj (json_object *): The json object, who's timestamp is to be updated
 *      name (const char *): Key the time stamp is stored under.
 *      timestamp (const struct timespec *): The timespec data to be encoded and stored.
 * Returns:
 *      - `NULL` in Error cases
 *      - Pointer to the json array containing the timestamp.
 ******************************************************************/
struct json_object *safuJsonAddNewTimestamp(struct json_object *jobj, const char *name,
                                            const struct timespec *timestamp);

/*******************************************************************
 * Retrieve a timespec struct from an json object.
 *
 * Parameters:
 *      jobj (json_object *): The json object from which a timestamp is to be read
 *      name (const char *): Key the timestamp is stored under,
 *              if the given json object is an object(map)
 *      val (size_t *): Index of the timestamp, if the given json object
 *              is an array
 *      timestamp (const struct timespec *): The timespec in which the data should be stored
 * Returns:
 *      - `NULL` in Error cases
 *      - Pointer to the json array containing the timestamp.
 * Returns:
 *      - `0` in success case
 *      - `-1` in error case
 ******************************************************************/
int safuJsonGetTimestamp(const struct json_object *jobj, const char *name, size_t idx, struct timespec *timestamp);
