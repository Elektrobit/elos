# cmocka_mocks

Library to mock common C functions, including libc and jsonc,
for testing purposes.

## Building

cmocka_mocks uses CMake for building:

```bash
cmake -B ./build ./cmocka_mocks
make -C ./build all
make -C ./build install
```

or use the CI hooks

```bash
./cmocka_mocks/ci/build.sh [Release]
```

## Folders

### ci

Scripts to be used by CI pipelines to build , run various tests and checks.

### documentation

A more detailed description of the mocked functions can be found in the documentation.

```
./cmocka_mocks/documentation/documentation.md
```

### src

Contains the actual productive sources.

#### src/cmocka_mocks

Contains code.
