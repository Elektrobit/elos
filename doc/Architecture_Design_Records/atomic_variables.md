# Architecture Design Record - Atomic Variables

## Problem

Elos is a multithreaded application that shares a lot of its important data
structures amongst various threads. So far we have been using mutexes,
which work overall but come with their own set of problems - They have no way
of checking if a mutex variable is initialized, while mutex locking itself
makes the code harder to read due to introducing additional complexity.

C11 added an atomic variable functionality which could help with the above
problems while being potentially faster too.

Are atomic variables a viable alternative to mutexes?

## Goals

* At least a basic understanding of atomic variables
* Potential pitfalls
* Potential uses
* Find out if we can replace mutexes with atomic variables

## Considerations

### Documentation

The documentation regarding atomic variables is sparse at best and usually
very technical, especially when it comes to memory order. The feature
originates from C++11, with C - and interestingly Rust - basing their
implementations directly on it. Especially for C there is hardly any
direct documentation available, so a few things have to be inferred from
alternative sources (C++ and Rust).

### Platform dependency

The feature depends heavily on the used processor architecture,
as the atomic commands get converted into appropriate assembler instructions.
Should a platform lack support for a certain variable type, a soft implementation
will be utilized that can be expected to be much slower. This will not happen
silently however, as there will be missing symbols during the linking stage
that have to be provided via linking `libatomic` into the executable.

### Creating and accessing atomic variables

A list of types and commands can be found under https://en.cppreference.com/w/c/atomic

An atomic variable can be created by adding the `_Atomic` attribute to a variables
or by using one of the predefined types, e.g. `atomic_uint_fast64_t`.
The `_Atomic` prefix works for every type of variable, even structs.
A potential result of this is however the necessity to link `libatomic` and very
poor performance (much worse than with Mutexes).

Read and write operations can be done implicitely (e.g. `atomicVar += 1`)
or with special macro operations (e.g. `atomic_fetch_add(&atomicVar, 1)`).
Implicit read/write can't be made atomic in all cases, notable "not working"
cases are for example `~atomicVar` or `++atomicVar`. This also extends
to structs with the `_Atomic` prefix, which cannot be read and modified in one
atomic step at all.

**Conclusion:**
* We should rely only on the given variables types
(like `atomic_uint_fast64_t`) instead of using the `_Atomic` attribute.
* We should only use the macro operations like `atomic_fetch_add` and limit
ourself to what is available here.

Simple code example for using an atomic variable:

```C
    #include <pthread.h>
    #include <stdatomic.h>
    #include <stdio.h>

    #define THREADS    2
    #define ITERATIONS 1000000

    static void *incrementWorker(void *ptr) {
        atomic_uint_fast64_t *atomicVar = (atomic_uint_fast64_t *)ptr;

        for (int i = 0; i < ITERATIONS; i += 1) {
            atomic_fetch_add(atomicVar, 1);
        }

        return NULL;
    }

    int main(void) {
        atomic_uint_fast64_t atomicVar = 0;
        pthread_t thread[THREADS] = {0};
        int result = -1;

        for (int i = 0; i < THREADS; i += 1) {
            result = pthread_create(&thread[i], 0, incrementWorker, &atomicVar);
            if (result < 0) {
                perror("pthread_create");
                break;
            }
        }

        if (result == 0) {
            for (int i = 0; i < THREADS; i += 1) {
                result = pthread_join(thread[i], NULL);
                if (result < 0) {
                    perror("pthread_create");
                    break;
                }
            }

            printf("Using %u threads, expected: %u, received: %lu\n", THREADS, (THREADS * ITERATIONS), atomicVar);
        }

        return result;
    }
```

**Note:** The sources are available under https://gitlabintern.emlix.com/csteiger/atomic_examples

### Memory order and fences

Memory order and fences will we excluded from this document for now,
as understanding both topics properly requires a deeper dive into
technical reference manuals of x86 and ARM, for which we are currently lacking the time.

There also is an oddity at least with x86/GCC, which seems to ignore the
memory order parameter when given dynamically, as seen below in the code snippet.

```
    static void *incrementValueWorker(void *ptr) {
        187b:	f3 0f 1e fa          	endbr64
        incrementData_t *data = (incrementData_t *)ptr;

        for (int i = 0; i < ITERATIONS; i += 1) {
        187f:	b8 00 00 00 00       	mov    $0x0,%eax
        1884:	eb 0c                	jmp    1892 <incrementValueWorker+0x17>
            atomic_fetch_add_explicit(&value, 1, data->order);
        1886:	f0 48 83 05 e1 2a 00 	lock addq $0x1,0x2ae1(%rip)        # 4370 <value>
        188d:	00 01
        for (int i = 0; i < ITERATIONS; i += 1) {
        188f:	83 c0 01             	add    $0x1,%eax
        1892:	3d 3f 42 0f 00       	cmp    $0xf423f,%eax
        1897:	7e ed                	jle    1886 <incrementValueWorker+0xb>
        }

        return NULL;
    }
        1899:	b8 00 00 00 00       	mov    $0x0,%eax
        189e:	c3                   	ret
```

Here the `atomic_fetch_add_explicit()` is converted to `lock addq $0x1,0x2ae1(%rip)`
which does lack any sort of variability that one would expect. We have to watch this potential
issue closely once we find the time to take a second look at the memory order.
It might be only a suggestion for the compiler, or might not work at all if specified during runtime.

*Note:* The memory order defaults to "sequentially consistent ordering"
(`memory_order_seq_cst`) if not specified

### Performance

Atomic variables are a lot faster than mutexes as long as the soft
implementation from `libatomic` is not used. See the table below, which contains
the runtime for 2 threads with 1000000 increments each. As the times are
varying wildly due to multithreading, average values from 20 iterations have
been calculated.

| incrementFunc       | timeTakenInSeconds |
| ------------------- | ------------------ |
| U32Unsafe           | 0.003247s |
| U32Mutex            | 0.264180s |
| U32Atomic           | 0.048557s |
| U64Atomic           | 0.058961s |
| ComplexMutex        | 0.261543s |
| ComplexAtomicSeqCst | 0.640935s |

Here we can see that atomic variables for an uint32 are around ~5.4 times
faster than a mutex. An example for the poor performance with soft implementation
can be seen with the "Complex" test (using a simple struct with a couple
of dummy entries), which is a lot slower than mutexes.

## Conclusion

Atomic variables can not replace mutexes. Only certain variable types and
operations are supported, and there is no way to lock a variable between a read
with subsequent operations and a write.

They are however much simpler and faster than mutexes and can be used in
a variety of places. Examples for this are state and/or flag fields that
define a components state, allowing to easily circumvent the biggest problem
with mutexes - not knowing if a mutex is initialized or not.

As such we gain the most by using atomic variables together with mutexes,
using each where it provides the most benefits.

### Usage Guidelines

We should use atomic variables when:

* We have a simple integer based value (e.g. a state enum, a flag field, a counter) that can be accessed in paralell
* Said value can represented by one of the types in `stdatomic.h`, e.g. `atomic_uint_fast64_t`
* We are able to realize all the interactions with the value exclusively with the macros given in `stdatomic.h`, e.g. `atomic_fetch_add()`

We should use mutexes when:

* We have data structures (`struct`) that can be accessed in paralell
* We have values that can be accessed in parallel which can't be represented directly by `stdatomics.h`, e.g. `float` or `double`
* We need to interact with this value in ways that can't be represented with macros from `stdatomics.h`, e.g. `++value` or `~value`
* We have more complex interactions that can't be done in one step, e.g. *read value -> do something specific based on value -> write value*
