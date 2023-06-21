# RpnFilter

## Introduction

The RpnFilter is stack based state machine that is intended to filter elosd events.

## Filter Syntax

The Filter Syntax is based on RPN (Reverse Polish Notation, used for example in some HP calculators),
a very simple stack based notation style that is easy to parse while still being able to do everything
the normal algebraic notation style can do.

The notation works by putting values on a stack (e.g. 1, 42) that are followed by a command (e.g. EQ).
The command will consume one or more values from the stack (most consume 2 values, e.g. ADD and EQ)
and place its own return value on it, which, in turn, can be consumed by commands that follow it.

A filter string of "1 1 EQ" would compare 1 with 1 (both values get removed from the stack) and insert a 1 (for true)
in turn.

A filter string of "1 2 ADD 4 EQ" would first place 1 and 2 on the stack, add them (which removes 1 and 2
and places a 3 on the stack instead) place a 4 after the ADD's 3 and then compare the two values (3 == 4),
removing the values and placing a 0 (false) on the stack after it.

### Semantic rules

The RpnFilter currently supports 4 different semantics: Numbers, Strings, Locations and Commands.

#### Numbers

Any number that would be valid as INT32, e.g. 42

#### Strings

A sequence of characters bracketed by ' or ", e.g. 'foo', 'test123', 'hello world', "foo", "foo 'bar' baz", 'foo "bar" baz'.
Note: Character escaping (like \" or \') is currently not supported.

#### Locations

Locations are written with a '.' followed by a positive number (e.g. ".1").
These are intended for multiple purposes, but their scope is currently limited to accessing the input parameters
that are given to the filter when its run. A ".1" refers to the first input parameter, a ".2" to the second and so on.

#### Commands

A sequence of [A-Z] characters that indicate the operations to run with the data on the stack (e.g. ADD, EQ).

Currently supported commands:

* EQ  ->  equal (==)
* NE  ->  not equal (!=)
* LT  ->  lesser than (<)
* GT  ->  greater than (>)
* LE  ->  lesser or equal (<=)
* GE  ->  greater or equal (>=)

* AND  ->  bitwise AND (&&)
* OR   ->  bitwise OR (||)
* XOR  ->  bitwise XOR (^)
* NOT  ->  bitwise NOT (~)

* ADD  -> addition (+)
* SUB  -> substraction (-)
* DIV  -> division (/)
* MUL  -> multiplication (*)

* STRCMP -> string compare (strcmp())

* REGEX -> string analysis (regexec())

#### Examples

Here a few basic examples with their C-Style notation to the right:

```
    .1 1 EQ                ==>  (input->entry[0] == 1)
    1 2 ADD 3 SUB 0 EQ     ==>  (((1 + 2) - 3) == 0)
    .1 64 EQ .2 42 EQ AND  ==>  ((input->entry[0] == 64) && (input->entry[1] == 42))
    .1 'foo' STRCMP        ==>  (strcmp(input->entry[0], "foo") == 0)
```

## Interface

The following functions are defined for the RpnFilter interface

### elosRpnFilterInit

Initializes an existing elosRpnFilter_t.

Info: Needs to be freed with elosRpnFilterDeleteMembers() after usage

Function:

```c
elosRpnFilterResultE_t elosRpnFilterInit(
       elosRpnFilter_t *filter
   )
```

Parameters:
* `[in+out] ` filter -> Pointer a an existing elosRpnFilter_t that will be initialized

Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterNew

Allocates an elosRpnFilter.

Info: Needs to be freed with `elosRpnFilterDelete()` after usage

Function:

```c
elosRpnFilterResultE_t elosRpnFilterNew(
       elosRpnFilter_t **filter
   );
```

Parameters:
* `[in+out] ` filter -> Pointer to a pointer that will contain the newly allocated elosRpnFilter after calling the function

Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterDeepCopy

Makes a deep copy of elosRpnFilter.
The copy is completely independent of the original.

Info: 'to' needs to be freed with elosRpnFilterDeleteMembers() after usage

Function:

```c
elosRpnFilterResultE_t elosRpnFilterDeepCopy(
       elosRpnFilter_t *to,
       const elosRpnFilter_t *from
   );
```

Parameters:
* `[in+out] ` to -> pointer to where the elosRpnFilter_t shall be copied
* `[in] `     from -> pointer to elosRpnFilter_t that shall be copied from


Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterClone

Duplicates elosRpnFilter into a newly allocated memory region.
The copy is completely independent of the original and needs to be freed separately.

Info: 'to' needs to be freed with elosRpnFilterDelete()

Function:

```c
elosRpnFilterResultE_t elosRpnFilterClone(
       elosRpnFilter_t **to, 
       const elosRpnFilter_t *from
   );
```

Parameters:
* `[in+out] ` to -> pointer to a pointer which will contain the new elosRpnFilter_t
* `[in] `     from -> pointer to elosRpnFilter_t that shall be copied from


Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterDeleteMembers

Frees memory used by elosRpnFilter members

Info: 

Function:

```c
elosRpnFilterResultE_t elosRpnFilterDeleteMembers(
        elosRpnFilter_t *filter
   );
```

Parameters:
* `[in+out] ` filter -> Pointer to the elosRpnFilter_t to free

Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterDelete

Frees memory used by a elosRpnFilter_t

Info: 

Function:

```c
elosRpnFilterResultE_t elosRpnFilterDelete(
        elosRpnFilter_t *filter
    );
```

Parameters:
* `[in+out] ` filter -> Pointer to the memory area that will be freed

Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterCreate

Create new filter to be used with elosRpnFilterExecute

Info: Needs to be freed after use with elosRpnFilterDeleteMembers()

Function:

```c
elosRpnFilterResultE_t elosRpnFilterCreate(
        elosRpnFilter_t *filter,
        const elosRpnFilterParam_t *param
    );
```

Parameters:
* `[in+out] ` filter -> Initialized elosRpnFilter and parameters
* `[in] `      param -> Filter creation parameters, including a filter string

Returns:
* FILTER_RESULT_OK for success, FILTER_RESULT_ERROR on failure

### elosRpnFilterExecute

Runs an elosRpnFilter on the given data

Info: 

Function:

```c
elosRpnFilterResultE_t elosRpnFilterExecute(
        const elosRpnFilter_t *filter,
        const elosRpnFilterStack_t *input
    );
```

Parameters:
* `[in] ` filter -> elosRpnFilter_t created with elosRpnFilterCreate()
* `[in] ` input -> A stack with values to be used with the filter

Returns:
* FILTER_RESULT_MATCH on success with a match, FILTER_RESULT_NO_MATCH on success with no match, FILTER_RESULT_ERROR on failure



### elosRpnFilterExecuteResult

Runs an elosRpnFilter on the given data and returns the remaining values on the filter stack.

Info: 

Function:

```c
elosRpnFilterResultE_t elosRpnFilterExecuteResult(
        const elosRpnFilter_t *filter,
        const elosRpnFilterStack_t *input,
        elosRpnFilterStack_t *output
    );
```

Parameters:
* `[in] ` filter -> elosRpnFilter_t created with elosRpnFilterCreate()
* `[in] ` input -> A stack with values to be used with the filter
* `[out] ` output -> A stack with values generated by the filter

Returns:
* FILTER_RESULT_MATCH on success with a match, FILTER_RESULT_NO_MATCH on success with no match, FILTER_RESULT_ERROR on failure

## Usage

Here's a simple example that will do a string compare.

Note: There will be helper functions for stack building and handling in the future.

```c
    // Create the filter
    elosRpnFilter_t filter = ELOS_RPNFILTER_INIT;
    elosRpnFilterParam_t param = {.filterString = ".1 'foo' STRCMP"};

    elosRpnFilterCreate(&filter, &param);

    // Execute the filter, for which we need to create a stack of values that is passed into the rpnfilter
    elosRpnFilterResultE_t result;
    size_t stackSize = 1;
    size_t stackMemSize = sizeof(elosRpnFilterStack_t) + (sizeof(elosRpnFilterStackEntry_t) * stackSize);
    elosRpnFilterStack_t *input = safuAllocMem(NULL, stackMemSize);
    input->count = stackSize;
    input->entry[0].type = FILTER_VALUE_STRING;
    input->entry[0].data = "foo";

    result = elosRpnFilterExecute(&filter, input);
    if (result == FILTER_RESULT_MATCH) {
        printf("match!");
    } else {
        printf("no match!");
    }

    // Free the resources used by the filter object
    elosRpnFilterDeleteMembers(&filter);
```
