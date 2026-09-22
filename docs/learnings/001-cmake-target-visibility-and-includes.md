# Learning 001: Modern CMake Target Visibility and Include Resolution

## Context
Setting up the foundational build system (Milestone 0) for the `gateway` component using target-based CMake (>= 3.20) and C++20.

## What was the problem?
During the initial build configuration and compilation, two distinct issues occurred:
1. `CMake Error: Cannot determine link language for target "gateway_lib"`.
2. Header lookup failed (`fatal error: gateway/version.hpp: No such file or directory`) during the compilation of `gateway_app`.

## What was investigated / tried?
1. Declared `gateway_lib` as an `INTERFACE` target when no `.cpp` translation units were present.
2. Linked `gateway_lib` into the executable using:
   ```cmake
   target_link_libraries(gateway_app INTERFACE gateway_lib)
   ```
3. Inspected the generated `build/compile_commands.json` to verify whether the `-I` include search flags were passed to GCC.

## What happened and why? (Root Cause)

### 1. Linker Language Deduction
`add_library(target)` without a specified type expects at least one source file with a recognizable extension (e.g. `.cpp`). CMake inspects file extensions to choose the proper compiler and linker (GCC vs. G++). If no translation unit is provided, CMake cannot deduce the linker language and aborts generation.

### 2. CMake Property Propagation Semantics
CMake manages compiler options, definitions, and include directories via Target Properties. The visibility specifiers define how these properties propagate:

| Specifier | Applied to Target Itself? | Propagated to Consumers? |
|---|---|---|
| `PRIVATE` | **Yes** | No |
| `INTERFACE` | No | **Yes** |
| `PUBLIC` | **Yes** | **Yes** |

### 3. The Executable Trap
Using `target_link_libraries(gateway_app INTERFACE gateway_lib)` instructed CMake:
> *"Do not apply `gateway_lib`'s include directories to `gateway_app` itself; only propagate them to targets that link against `gateway_app`."*

Because an executable (`add_executable`) is a terminal binary artifact and never linked by downstream targets, the include paths from `gateway_lib` were completely discarded. GCC was invoked without `-I/path/to/gateway/include`.

## Key Learnings & Rules of Thumb

* **Executables always link `PRIVATE`:**
  ```cmake
  target_link_libraries(gateway_app PRIVATE gateway_lib)
  ```
  An executable is an end-of-the-line consumer. It should almost never propagate dependencies using `INTERFACE` or `PUBLIC`.

* **Static/Shared Libraries with Headers use `PUBLIC`:**
  When a library's own `.cpp` files include headers that external consumers also require, use:
  ```cmake
  target_include_directories(gateway_lib PUBLIC ...)
  ```

* **Namespaced Header Layout:**
  Always mirror the namespace in the filesystem (`include/gateway/version.hpp`). Including via `<gateway/version.hpp>` prevents namespace collisions when integrating third-party libraries or building in complex Yocto multi-layer environments.