# safu

Library of common utility API's used in elos and samconf.

## Building

safu uses CMake for building:

```bash
cmake -B ./build ./safu
make -C ./build all
make -C ./build install
```

or use the CI hooks

```bash
./safu/ci/build.sh [Release]
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


### src

Contains the actual productive sources.

#### src/safu

Contains code.

### test

Contains all test related sources. See the documenation for details.
