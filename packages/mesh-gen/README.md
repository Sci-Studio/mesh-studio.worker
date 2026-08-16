# mesh-gen

C++ library and CLI for 2D mesh generation from CAD outline geometry.

Given a DXF file, **mesh-gen** parses supported entities into points and constraint edges, builds a Delaunay triangulation, and writes an SVG preview.

Sample DXF files live in [`data/`](data/).

## Requirements

- **CMake** ≥ 3.14
- **C++20** compiler (GCC 10+, Clang 12+, or MSVC with C++20)
- Network on first configure (GoogleTest is fetched via CMake `FetchContent`)

Optional for local development:

- **clangd** — IDE navigation / diagnostics (uses `compile_commands.json`)
- **clang-tidy** — naming checks (see [`.clang-tidy`](.clang-tidy))

## Build

From this directory (`packages/mesh-gen`):

```bash
cmake -S . -B build
cmake --build build
```

This produces:

- `build/libMeshGen.so` — shared library
- `build/meshgen` — CLI
- `build/meshgen_tests` — GoogleTest binary

`CMAKE_EXPORT_COMPILE_COMMANDS` is enabled, so CMake also writes `build/compile_commands.json`.

## Build in debug mode

```bash
cmake -S . -B build-debug -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

## Build with Address Sanitizer
From this directory (`packages/mesh-gen`):
```bash
cmake -S . -B build-asan -DCMAKE_BUILD_TYPE=Debug -DENABLE_SANITIZERS=ON
cmake --build build-asan
```

## Build for performance profiling
From this directory (`packages/mesh-gen`):
```bash
cmake -S . -B build-profile \ 
    -DCMAKE_BUILD_TYPE=RelWithDebInfo \
    -DCMAKE_CXX_FLAGS_RELWITHDEBINFO="-O2 -g -fno-omit-frame-pointer"
cmake --build build-profile
```

## Run

```bash
./build/meshgen path/to/input.dxf
# for build-asan:
./build-asan/meshgen data/rectangle-prism-Shape2DView.dxf
# or:
ASAN_OPTIONS=detect_leaks=1:halt_on_error=1 ./build-asan/meshgen data/rectangle-prism-Shape2DView.dxf
```

Example with a bundled fixture:

```bash
./build/meshgen data/rectangle-prism-Shape2DView.dxf
```

On success, open the SVG under `tmp/` (filename matches the DXF stem).


## Run valgrind
 - use the build debug for a valgrind run
```bash
valgrind \
    --tool=memcheck \
    --leak-check=full \
    --show-leak-kinds=all \
    --track-origins=yes \
    ./build-debug/meshgen data/rectangle-prism-Shape2DView.dxf
```

## Run perf
```bash
sudo perf record \
    --call-graph dwarf \
    ./build-profile/meshgen data/model.dxf
sudo perf report
```

## Test

```bash
ctest --test-dir build --output-on-failure
# or:
./build/meshgen_tests
```

## Development tips

### `compile_commands.json` (clangd)

Clangd looks for `compile_commands.json` at the project root. After the first configure:

```bash
ln -sf build/compile_commands.json compile_commands.json
```

This repo already expects that symlink (it is gitignored). Re-run the link if you delete `build/`.

A small [`.clangd`](.clangd) file can add extra compile flags when the system headers need help; keep it machine-local if paths differ across developers (it is gitignored here).

### clang-tidy

With a compilation database in place:

```bash
clang-tidy -p build src/MeshGen.cpp
```

Checks are configured in [`.clang-tidy`](.clang-tidy).

