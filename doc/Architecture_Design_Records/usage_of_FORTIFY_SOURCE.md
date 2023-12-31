# Design Decisions - Enforce usage of \_FORTIFY_SOURCE

## Problem
The libc defines in features.h the compiler define \_FORTIFY_SOURCE witch can have different levels 0 to 2.  
Taken from /usr/include/features.h

```
   _FORTIFY_SOURCE      Add security hardening to many library functions.
                        Set to 1 or 2; 2 performs stricter checks than 1.
```

This leads the library header to make several optimizations and replacements of functions to add additional compile time and runtime checks. In case of the *recv* function this looks like:

``` c
__fortify_function ssize_t
recv (int __fd, void *__buf, size_t __n, int __flags)
{
  size_t sz = __glibc_objsize0 (__buf);
  if (__glibc_safe_or_unknown_len (__n, sizeof (char), sz))
    return __recv_alias (__fd, __buf, __n, __flags);
  if (__glibc_unsafe_len (__n, sizeof (char), sz))
    return __recv_chk_warn (__fd, __buf, __n, sz, __flags);
  return __recv_chk (__fd, __buf, __n, sz, __flags);
}
```

Currently it is not defined whether we expected to use this additional safety checks or not. This leads us to the problem that we need to decide in the Unit Tests which function call is expected i.e. *recv* or \_\_recv_chck or one of the other alternatives.

see also: https://www.redhat.com/en/blog/enhance-application-security-fortifysource
https://wiki.debian.org/Hardening#DEB_BUILD_HARDENING_FORTIFY_.28gcc.2Fg.2B-.2B-_-D_FORTIFY_SOURCE.3D2.29


## Influencing factors
The following constraints have to be taken into account for a suitable solution:
  - Performance of code using _FORTIFY_SOURCE

### Mocking in Unit-Tests
The FORTIFY_SOURCE mechanics are based mostly on macros and redirections (wrapper) to some other implementation of functions. For unit testing we have to know this and explicitly write test that expect and check for other method calls as defined in the code under test. see example above.

In addition if we agree on using FORTIFY_SOURCE in any level, this becomes a dependency for our projects. Otherwise the unit tests will fail as they have to be adopted to expect special or other function calls then the code under test suggests.

### libC Dependencies
As a final consequence, projects are only usable on platforms with a libc-implementation which has FORTIFY_SOURCE enabled on given level and is compliant to the one used during development.

## Assumptions
The following assumptions have been made in the decision process:
  - \_FORTIFY_SOURCE is configured during all builds, including the Release build. Hence also on the target the code is using \_FORTIFY_SOURCE.

libC (e.g. glibc) shall support \_FORTIFY_SOURCE

## Considered Alternatives

### 1\) Enforce \_FORTIFY_SOURCE == 1
If _FORTIFY_SOURCE is set to 1, with compiler optimization level 1 (gcc -O1) and above, checks that shouldn't change the behavior of conforming programs are performed. 

**pros**
  - basically recommended to use
      - But: \_FORTIFY_SOURCE == 2 is even more recommended

**cons**
  - (none)

### 2\) Enforce \_FORTIFY_SOURCE == 2
With _FORTIFY_SOURCE set to 2  some  more  checking  is added, but some conforming programs might fail.  Some of the checks can be performed at compile time, and result in compiler warnings; other checks take place at run time, and result in a run-time error if the check fails.

**pros**

  - More recommended than \_FORTIFY_SOURCE == 1
  - standard way in the community
  - provides most secure results 

**cons**

  - Might cause issues (some conforming programs might fail.)
      - But: The tests should detect these fails.

### 3\) Enforce \_FORTIFY_SOURCE == 0 (Disable)
description of this alternative.

**pros**

  - (none)

**cons**

  - buffer over flows might be left undetected.

## Decision
Option 2 is chosen.

### Rationale

### Open Points

none



