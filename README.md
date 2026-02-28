# vmti

Standalone CMake project for developing and testing `VMTIMetadata` and `VMTIBuilder`.

## Build

```bash
cmake -S . -B build
cmake --build build
```

## Test

```bash
ctest --test-dir build --output-on-failure
```
