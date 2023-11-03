# Architecture Design Record - Shared Memory Locking

## Problem

We need to read and write data from a shared memory region that is shared
across multiple virtual machines (using /dev/mem). To prevent data races,
we need some sort of locking mechanism that works with /dev/mem.

## Considerations

There are a few methods for locking, which will be detailed in this section.

### Kernel based locking (Mutex, Futex, Semaphores, ...)

There are a couple of good options with "classic" shared memory allocated
via the POSIX functions calls, e.g. pthread_mutex or the newer futex,
with semaphores being an additional option if the locking mechanism doesn't
need to be placed inside the shared memory.

None of these are designed to work with a concept like /dev/mem though,
which is accessed by multiple virtual machines that have each their own kernel,
while the locking mechanism themselves are managed by only one kernel.

Technically a pthread_mutex or a futex could be placed inside /dev/mem,
but this is veering deeply into "undefined behavior" territory and should be
avoided at all costs. In the worst case it will seem like it is working and
will start breaking when we least expect it further down the line.

### Spinlock (Atomic variables)

Atomic variables are not managed by a kernel and work fine as long as
the operation can be translated directly into assembler code. This shouldn't
be a problem for the usual basic integer operations up to the processors
register size (e.g. 8 bytes on 64bit platforms).

As a spinlock makes use of atomic integers we can use it for this use case.
The issue here is with the spinlock itself which has a whole host of issues
that cannot be ignored for normal use cases (e.g. driving a whole core to
a 100% usage just for waiting until something happens).

### Hypervisor based locking

Hypervisor based locking is the most complicated, but really the only option
to properly implement a locking mechanism for an address range shared across
multiple machines.

Roughly generalized, we have two possibles modes of operation detailed below.

#### Adress range based locking

The hypervisor mode can be set to trigger on a predefined address range;
This is usually used to virtualize or emulate hardware, but, in theory,
might be used here as well. The data structure itself would be managed by
the hypervisor in this case, with the users having their own simplified view
of the data with the ability to read or write data entries.

#### Software interrupt based locking

The hypervisor mode has its own version of an software interrupt,
which is very similiar to the normal software interrupt that is
used to implement the kernel system calls.

This feature could be used to implement a mutex-like construct that does
support locking across multiple virtual machines properly.

## Conclusion

Only spinlocks are currently available as a safe and ready to use locking
method in conjunction with /dev/mem, in the long term hypervisor based locking
should be designed and realized due to its inherent advantages.
