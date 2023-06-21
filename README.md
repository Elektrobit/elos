# Event Logging and Management System (elos)

Daemon that monitors different events and makes them available
via request to client applications

# Documentation

The elos documentation can be found here: https://elektrobit.pages.emlix.com/base-os/corbos-tools/

## Building

elos uses CMake for building:

```bash
cmake -B ./build ./elos
make -C ./build all
make -C ./build install
```

or use the CI hooks

```bash
./elos/ci/build.sh [Release]
```

### Workspace setup

1. Please run the following git commands after cloning the repository:

   ```bash
   git config core.hooksPath .githooks
   ```

   to set the commit message template and to setup the project specific git hooks.

2. Usually it is a good idea to create a symlink to the compilation database
   created by cmake in the build directory. Run therefore:

   ```
   ln -s build/compile_commands.json
   ```

## Folders

### ci

Scripts to be used by CI pipelines to build , run various tests and checks.

### documentation

More detailed documentation on the event logging and management system.

### packaging

Contains some files to help out some external build systems which are not part of this project.

## src

Contains the actual productive sources.

#### src/common

Contains common shared code.

#### src/demo

Contains simple demonstration applications.

#### src/elosd

The sources for the event logging and management system daemon itself.

#### src/libelos

Contains header and source files for a shared library that can be used
to communicate with the event logging and management system daemon (elosd).

#### src/log

A log library used by elos to consolidate logging.

#### src/scanner

Contains the sources for event logging and management system specific shared libraries,
these are responsible for monitoring for different events on the target system.

#### src/version

The sources to manage the version always in the same way.

### test

Contains all test related sources. See the documenation for details.
