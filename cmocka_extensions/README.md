# cmocka_extensions

A collection of mocking related code extensions needes by other components.

## Building

cmocka_extensions uses CMake for building:

```bash
cmake -B ./build ./cmocka_extensions
make -C ./build all
make -C ./build install
```

or use the CI hooks

```bash
./cmocka_extensions/ci/build.sh [Release]
```

## Folders

### ci

Scripts to be used by CI pipelines to build , run various tests and checks.

### documentation

More detailed documentation of the mocked functions.

### src

Contains the actual productive sources.

#### src/cmocka_extensions

Contains code.
