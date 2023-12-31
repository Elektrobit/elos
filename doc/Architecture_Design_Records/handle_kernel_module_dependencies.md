# Handle kernel module dependencies for Elos

## Description

Some Elos functionality depends on kernel modules to be loaded. In general
there are three possible scenarios for required kernel modules:

1. Module (aka feature) is compiled in
   * The module is available and no further actions are necessary.
2. Module is compiled as loadable module and needs to be loaded at runtime by
   `init_module` system call.
   * The module is available in the file system on the target, but someone has
     to load it at some point into the kernel to make it available.
3. Module is not available and/or compiled at all
   * The module is not available at all, means missing dependencies and it
     needs to be clarified how to proceed in this case.


## Solutions

### 1) Compiled in Module

For compiled in modules there is nothing to do, the required modules  (aka features) will be
available as `init` is started by the kernel.

### 2) Module as loadable modules

In this case the `xy.ko` file needs to be loaded at some point by someone
during runtime to be available when Elos requires the functionality.   
This can be done by:

#### 2.1) Leave it to the init system to load all required modules at once on boot

* Modules to load on boot are defined by `/etc/modules` or
  `/etc/modules-load.d/<module_name>.conf`as described in `man 5 modules` or
  `man 5 modules-load.d`
* Default way in Linux Systems to load modules (see
  https://www.freedesktop.org/software/systemd/man/modules-load.d.html)
* consider udev and special udev rules:
  if netlink becomes available load all netlink modules available
* **Downside is**, this increase the time for the system to become ready as all
  probably not immediately necessary modules are loaded at boot time at once

#### 2.2) Configure the init system to manage kernel module loading depending on tasks requirements

* It's up to the init system to decide when to load a module. This can
  shorten the time the system needs to become ready
* Systemd loads modules from `/etc/modules/` aka
  `/etc/modules-load.d/<module_name>.conf` via
  [libkmod](http://git.kernel.org/pub/scm/utils/kernel/kmod/kmod.git)
* `crinit` could load modules configured as task via modprobe to avoid
  reimplementing the wheel, alternative is to use libkmod like systemd
 * All dependencies and requirements can be described and controlled within
   a crinit task file
 * Module dependencies can be managed at a central point by crinit for all
   process that have (the same) module dependencies
* consider udev and special udev rules:
  if netlink becomes available load all netlink modules available

#### 2.3) Elos itself checks at a suitable point in time and loads the modules

* Elos can load modules by using `modprobe` or directly via libkmod.
* Elos test the kernel-API needed and on failure
  tries to load the needed modules (dependencies)
* Elos has to assume which kernel module provides the
  necessary functionality
* Further tools might need a kernel module and handle it this way. This will
  introduce some more topics to be
  discussed:
 * How to synchronize module management for modules used by different
   process?
 * Should and when can a module be unloaded?
 * Are there synchronization issues, if multiple process try to load the
   same module?
 * How to check kernel module parameter (module configuration)?
* Main contra points :
 * General purpose logic to load modules implemented in elos
 * Increased complexity because the need to synchronize kernel module
   loading with other processes

### 3) module is not available at all

In case the module is not available at all, neither compiled in nor provided as
loadable module (`*.ko`-file), Elos can provide two options:

#### 3.1) Probe and activate features at runtime

Elos shall detect and enable or disable certain features at runtime, depending
if required modules are available (loaded).

This requires Elos to:

A) Either be fault tolerant on failing functionality provided by some kernel
   modules and drop the support for all those functions and proceed as good as
   possible

B) Or to refuse any further operation and try to shutdown as graceful as
   possible

C) Combination of A) and B): If a feature is enforced in config but not
   available proceed in B), if config does not force a feature and it is not
   available try to proceed in A)

In any case Elos shall report the failing kernel calls as log messages and
provides a hint to the corresponding missing kernel feature.

#### 3.2) Static compile time configuration

Elos can be extended in a way that features which requires certain kernel
modules can be disabled. This will reduce the function set of Elos. It is not
impossible that another implementation can be found which provides the same
functionality like the required kernel module, but in this case it has usually
a performance impact or other reasons why the usage of a kernel module was be
preferred in the first place.

For package based Linux distributions this is not an option so we have to discard
this option.


## Decision/Rational

### Problem Scenario 1

For the first scenario we can't see any actions we have to take, thus we can
treat this solved for any target where required kernel modules are build in.

### Problem Scenario 2

We prefer for this scenario a solution based `2.2`. So the init-system
keeps track of necessary modules to load and the point in time when to load.
The module management shall be done centralized to avoid conflicts and
synchronisation efforts between multiple programs, each trying to load and
remove their kernel modules.
The kernel module loading procedure is also defined by the init-system used.

### Problem Scenario 3

If a module is not available `(problem 3)`, Elos shall use the approach `C)` in
`3.1`. For APIs like the `netlink` interface it is not easy or even possible to
decide between an error of the backend and a missing backend through a missing
kernel module. So we shall keep it simple and report the errors and proceed
with a reduced functionality and shutdown only in case a feature is configured
as "forced".

We shall also keep in mind that if Elos shuts down, there is probably no
remaining error reporting and tracing in the system at all. Therefore, we shall
keep up working and leave the sinking ship(target) as late as possible.

As mentioned in already in 3.2 static compile time configuration is not an
option and is discarded


## References

[libkmod](http://git.kernel.org/pub/scm/utils/kernel/kmod/kmod.git)
[modules-load.d](https://www.freedesktop.org/software/systemd/man/modules-load.d.html)
